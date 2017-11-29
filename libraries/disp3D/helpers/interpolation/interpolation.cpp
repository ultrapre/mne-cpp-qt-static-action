//=============================================================================================================
/**
* @file     interpolation.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017, Lars Debor and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Interpolation class definition.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "interpolation.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSet>
#include <QtDebug>



//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace DISP3DLIB;
using namespace Eigen;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// INITIALIZE STATIC MEMBER
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

SparseMatrix<float> Interpolation::createInterpolationMat(const QVector<qint32> &vecProjectedSensors,
                                                          const MatrixXd &matDistanceTable,
                                                          double (*interpolationFunction) (double),
                                                          const double dCancelDist,
                                                          const FIFFLIB::FiffInfo& fiffInfo,
                                                          qint32 iSensorType)
{

    if(matDistanceTable.rows() == 0 && matDistanceTable.cols() == 0) {
        qDebug() << "[WARNING] Interpolation::createInterpolationMat - received an empty distance table.";
        return SparseMatrix<float>();
    }

    // initialization
    SparseMatrix<float> matInterpolationMatrix(matDistanceTable.rows(), vecProjectedSensors.size());

    // temporary helper structure for filling sparse matrix
    QVector<Triplet<float> > vecNonZeroEntries;
    const qint32 iRows = matInterpolationMatrix.rows();
    const qint32 iCols = matInterpolationMatrix.cols();

    // insert all sensor nodes into set for faster lookup during later computation. Also consider bad channels here.
    QSet<qint32> sensorLookup;

    int idx = 0;

    for(const FIFFLIB::FiffChInfo& s : fiffInfo.chs){
        //Only take EEG with V as unit or MEG magnetometers with T as unit
        if(s.kind == iSensorType && (s.unit == FIFF_UNIT_T || s.unit == FIFF_UNIT_V)){
            if(!fiffInfo.bads.contains(s.ch_name)){
                sensorLookup.insert (vecProjectedSensors.at(idx));
            }

            idx++;
        }
    }

    int counter = 0;
    // main loop: go through all rows of distance table and calculate weights
    for (qint32 r = 0; r < iRows; ++r) {
        if (sensorLookup.contains(r) == false) {
            // "normal" node, i.e. one which was not assigned a sensor
            // bLoThreshold: stores the indizes that point to distances which are below the passed distance threshold (dCancelDist)
            QVector<QPair<qint32, float> > vecBelowThresh;
            float dWeightsSum = 0.0;
            const RowVectorXd& rowVec = matDistanceTable.row(r);

            for (qint32 c = 0; c < iCols; ++c) {
                const float dDist = rowVec[c];

                if (dDist < dCancelDist) {
                    const float dValueWeight = std::fabs(1.0 / interpolationFunction(dDist));
                    dWeightsSum += dValueWeight;
                    vecBelowThresh.push_back(qMakePair<qint32, float> (c, dValueWeight));
                }
            }

            for (const QPair<qint32, float> &qp : vecBelowThresh) {
                counter++;
                vecNonZeroEntries.push_back(Eigen::Triplet<float> (r, qp.first, qp.second / dWeightsSum));
            }
        } else {
            // a sensor has been assigned to this node, we do not need to interpolate anything (final vertex signal is equal to sensor input signal, thus factor 1)
            const int iIndexInSubset = vecProjectedSensors.indexOf(r);

            vecNonZeroEntries.push_back(Eigen::Triplet<float> (r, iIndexInSubset, 1));
            counter++;
        }
    }

    matInterpolationMatrix.setFromTriplets(vecNonZeroEntries.begin(), vecNonZeroEntries.end());

    return matInterpolationMatrix;
}


//*************************************************************************************************************

VectorXf Interpolation::interpolateSignal(const Eigen::SparseMatrix<float> &matInterpolationMatrix,
                                          const VectorXd &vecMeasurementData)
{
    if (matInterpolationMatrix.cols() != vecMeasurementData.rows()) {
        qDebug() << "[WARNING] Interpolation::interpolateSignal - Dimension mismatch. Return null pointer...";
        return VectorXf();
    }

    VectorXf pOutVec = matInterpolationMatrix * vecMeasurementData.cast<float>();

    return pOutVec;
}


//*************************************************************************************************************

double Interpolation::linear(const double dIn)
{
    return dIn;
}


//*************************************************************************************************************

double Interpolation::gaussian(const double dIn)
{
    return exp(-((dIn * dIn) / 2.0));
}


//*************************************************************************************************************

double Interpolation::square(const double dIn)
{
    return std::max((-(1.0f / 9.0f) * (dIn * dIn) + 1), 0.0);
}


//*************************************************************************************************************

double Interpolation::cubic(const double dIn)
{
    return dIn * dIn * dIn;
}
