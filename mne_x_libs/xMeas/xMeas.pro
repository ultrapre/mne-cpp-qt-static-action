#--------------------------------------------------------------------------------------------------------------
#
# @file     xMeas.pro
# @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
#           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
# @version  1.0
# @date     July, 2012
#
# @section  LICENSE
#
# Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that
# the following conditions are met:
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
#       following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
#       the following disclaimer in the documentation and/or other materials provided with the distribution.
#     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
#       to endorse or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# @brief    This project file builds the xMeas library.
#
#--------------------------------------------------------------------------------------------------------------

include(../../mne-cpp.pri)

TEMPLATE = lib

QT += widgets

DEFINES += XMEAS_LIBRARY

TARGET = xMeas
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

LIBS += -L$${MNE_LIBRARY_DIR}
CONFIG(debug, debug|release) {
    LIBS += -lMNE$${MNE_LIB_VERSION}Genericsd
}
else {
    LIBS += -lMNE$${MNE_LIB_VERSION}Generics
}

DESTDIR = $${MNE_LIBRARY_DIR}

#
# win32: copy dll's to bin dir
# unix: add lib folder to LD_LIBRARY_PATH
#
win32 {
    FILE = $${DESTDIR}/$${TARGET}.dll
    BINDIR = $${DESTDIR}/../bin
    FILE ~= s,/,\\,g
    BINDIR ~= s,/,\\,g
    QMAKE_POST_LINK += $${QMAKE_COPY} $$quote($${FILE}) $$quote($${BINDIR}) $$escape_expand(\\n\\t)
}

SOURCES += \
    Measurement/text.cpp \
    Measurement/realtimesamplearray.cpp \
    Measurement/realtimemultisamplearray.cpp \
    Measurement/progressbar.cpp \
    Measurement/numeric.cpp \
    Nomenclature/nomenclature.cpp \
    Measurement/IMeasurementSink.cpp \
    Measurement/IMeasurementSource.cpp \
    Measurement/sngchnmeasurement.cpp \
    Measurement/measurement.cpp \
    Measurement/mltchnmeasurement.cpp \
    Measurement/realtimemultisamplearray_new.cpp \
    Measurement/realtimesamplearraychinfo.cpp

HEADERS += \
    xmeas_global.h \
    Measurement/text.h \
    Measurement/realtimesamplearray.h \
    Measurement/realtimemultisamplearray.h \
    Measurement/progressbar.h \
    Measurement/numeric.h \
    Nomenclature/nomenclature.h \
    Measurement/IMeasurementSink.h \
    Measurement/IMeasurementSource.h \
    Measurement/sngchnmeasurement.h \
    Measurement/measurement.h \
    Measurement/mltchnmeasurement.h \
    Measurement/realtimemultisamplearray_new.h \
    Measurement/realtimesamplearraychinfo.h


INCLUDEPATH += $${EIGEN_INCLUDE_DIR}
INCLUDEPATH += $${MNE_INCLUDE_DIR}
INCLUDEPATH += $${MNE_X_INCLUDE_DIR}

# Install headers to include directory
header_files.files = ./*.h
header_files.path = $${MNE_X_INCLUDE_DIR}/xMeas

header_files_measurement.files = ./Measurement/*.h
header_files_measurement.path = $${MNE_X_INCLUDE_DIR}/xMeas/Measurement

header_files_nomenclature.files = ./Nomenclature/*.h
header_files_nomenclature.path = $${MNE_X_INCLUDE_DIR}/xMeas/Nomenclature

INSTALLS += header_files
INSTALLS += header_files_measurement
INSTALLS += header_files_nomenclature
