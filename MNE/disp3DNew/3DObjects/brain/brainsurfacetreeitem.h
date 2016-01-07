//=============================================================================================================
/**
* @file     brainsurfacetreeitem.h
* @author   Lorenz Esch <Lorenz.Esch@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     November, 2015
*
* @section  LICENSE
*
* Copyright (C) 2015, Lorenz Esch and Matti Hamalainen. All rights reserved.
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
* @brief     BrainSurfaceTreeItem class declaration.
*
*/

#ifndef BRAINSURFACETREEITEM_H
#define BRAINSURFACETREEITEM_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../../disp3DNew_global.h"

#include "../../helpers/abstracttreeitem.h"
#include "braintreeitem.h"

#include "../../helpers/types.h"
#include "../../helpers/renderable3Dentity.h"

#include "fs/label.h"


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QList>
#include <QVariant>
#include <QStringList>
#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================

#include <Eigen/Core>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DISP3DNEWLIB
//=============================================================================================================

namespace DISP3DNEWLIB
{


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Eigen;


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* BrainSurfaceTreeItem provides a generic brain tree item to hold of brain data (hemi, vertices, tris, etc.) from different sources (FreeSurfer, etc.).
*
* @brief Provides a generic brain tree item.
*/
class DISP3DNEWSHARED_EXPORT BrainSurfaceTreeItem : public AbstractTreeItem
{
    Q_OBJECT;

public:
    typedef QSharedPointer<BrainSurfaceTreeItem> SPtr;             /**< Shared pointer type for BrainSurfaceTreeItem class. */
    typedef QSharedPointer<const BrainSurfaceTreeItem> ConstSPtr;  /**< Const shared pointer type for BrainSurfaceTreeItem class. */

    //=========================================================================================================
    /**
    * Default constructor.
    *
    * @param[in] iType      The type of the item. See types.h for declaration and definition.
    * @param[in] text       The text of this item. This is also by default the displayed name of the item in a view.
    */
    explicit BrainSurfaceTreeItem(const int& iType = BrainTreeModelItemTypes::UnknownItem, const QString& text = "Surface");

    //=========================================================================================================
    /**
    * Default destructor
    */
    ~BrainSurfaceTreeItem();

    //=========================================================================================================
    /**
    * AbstractTreeItem functions
    */
    QVariant data(int role = Qt::UserRole + 1) const;
    void  setData(const QVariant& value, int role = Qt::UserRole + 1);

    //=========================================================================================================
    /**
    * Adds FreeSurfer data based on surface and annotation data to this item.
    *
    * @param[in] tSurface           FreeSurfer surface.
    * @param[in] parent             The Qt3D entity parent of the new item.
    *
    * @return                       Returns true if successful.
    */
    bool addData(const Surface& tSurface, Qt3DCore::QEntity* parent);

public slots:
    //=========================================================================================================
    /**
    * Call this slot whenever new colors for the activation data plotting are available.
    *
    * @param[in] sourceColorSamples     The color values for each estimated source.
    * @param[in] vertexIndex            The vertex idnex of each estiamted source.
    */
    void onRtVertColorUpdated(const QByteArray& sourceColorSamples, const VectorXi& vertexIndex);

private slots:
    //=========================================================================================================
    /**
    * Call this slot whenever the curvature color or origin of color information (curvature or annotation) changed.
    */
    void onColorInfoOriginOrCurvColorUpdated();

private:
    //=========================================================================================================
    /**
    * Cretes a QByteArray of colors for given curvature and color data.
    *
    * @param[in] curvature      The curvature information.
    * @param[in] colSulci       The sulci color information.
    * @param[in] colGyri        The gyri color information.
    */
    QByteArray createCurvatureVertColor(const VectorXf& curvature, const QColor& colSulci = QColor(50,50,50), const QColor& colGyri = QColor(125,125,125));

    Renderable3DEntity*     m_pRenderable3DEntity;                      /**< The renderable 3D entity. */
    Renderable3DEntity*     m_pRenderable3DEntityActivationOverlay;     /**< The renderable 3D entity used as an overlay for activity plotting. */

    //These are stored as member variables because we do not wat to look for them everytime we call functions, especially not when we perform rt source loc
    BrainTreeItem*          m_pItemSurfColorInfoOrigin;                 /**< The item which holds the information of the color origin (curvature or annotation). */
    BrainTreeItem*          m_pItemSurfColSulci;                        /**< The item which holds the sulci color information. */
    BrainTreeItem*          m_pItemSurfColGyri;                         /**< The item which holds the gyri color information. */
};

} //NAMESPACE DISP3DNEWLIB

#endif // BRAINSURFACETREEITEM_H
