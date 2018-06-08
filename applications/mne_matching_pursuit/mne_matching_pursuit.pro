#--------------------------------------------------------------------------------------------------------------
#
# @file     matchingPursuit.pro
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
#     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
#       to endorse or promote products derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# @brief    This project file builds the mne_matching_pursuit project
#
#--------------------------------------------------------------------------------------------------------------

include(../../mne-cpp.pri)

TEMPLATE = app

VERSION = $${MNE_CPP_VERSION}

QT += gui
QT += widgets  
QT += network core widgets concurrent
QT += xml

CONFIG   += console

TARGET = mne_matching_pursuit

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

LIBS += -L$${MNE_LIBRARY_DIR}
CONFIG(debug, debug|release) {
    LIBS += -lMNE$${MNE_LIB_VERSION}Utilsd \
            -lMNE$${MNE_LIB_VERSION}Fsd \
            -lMNE$${MNE_LIB_VERSION}Fiffd \
            -lMNE$${MNE_LIB_VERSION}Mned \
            -lMNE$${MNE_LIB_VERSION}Dispd
}
else {
    LIBS += -lMNE$${MNE_LIB_VERSION}Utils \
            -lMNE$${MNE_LIB_VERSION}Fs \
            -lMNE$${MNE_LIB_VERSION}Fiff \
            -lMNE$${MNE_LIB_VERSION}Mne \
            -lMNE$${MNE_LIB_VERSION}Disp
}

DESTDIR =  $${MNE_BINARY_DIR}

SOURCES += \
        main.cpp \
    editorwindow.cpp \
    enhancededitorwindow.cpp \
    formulaeditor.cpp \
    deletemessagebox.cpp \
    mainwindow.cpp \
    processdurationmessagebox.cpp \
    treebaseddictwindow.cpp \
    settingwindow.cpp

HEADERS += \
    editorwindow.h \
    enhancededitorwindow.h \
    formulaeditor.h \
    deletemessagebox.h \
    mainwindow.h \
    processdurationmessagebox.h \
    treebaseddictwindow.h \
    settingwindow.h

FORMS += \
    editorwindow.ui \
    enhancededitorwindow.ui \
    formulaeditor.ui \
    deletemessagebox.ui \
    mainwindow.ui \
    processdurationmessagebox.ui \
    treebaseddictwindow.ui \
    settingwindow.ui

RESOURCES += \
    Ressourcen.qrc

INCLUDEPATH += $${EIGEN_INCLUDE_DIR}
INCLUDEPATH += $${MNE_INCLUDE_DIR}

unix: QMAKE_CXXFLAGS += -isystem $$EIGEN_INCLUDE_DIR

# Deploy Qt Dependencies
win32 {
    EXTRA_ARGS =
    DEPLOY_CMD = $$WinDeployArgs($${TARGET},$${TARGET_EXT},$${MNE_BINARY_DIR},$${LIBS},$${EXTRA_ARGS})
    QMAKE_POST_LINK += $${DEPLOY_CMD}
    QMAKE_CLEAN += -r $$member(DEPLOY_CMD, 1)
}
unix:!macx {
    #ToDo Unix
}
macx {
    # === Mac ===
    QMAKE_RPATHDIR += @executable_path/../Frameworks
    EXTRA_LIBDIRS =

    # 3 entries returned in DEPLOY_CMD
    DEPLOY_CMD = $$MacDeployArgs($${TARGET},$${TARGET_EXT},$${MNE_BINARY_DIR},$${MNE_LIBRARY_DIR},$${EXTRA_LIBDIRS})
    QMAKE_POST_LINK += $${DEPLOY_CMD}
    QMAKE_CLEAN += -r $$member(DEPLOY_CMD, 1)

}
