# Stop at any error
set -e

# Location of the QT tools
if [ -z ${QTDIR+x} ]; then
	echo "QTDIR path not defined- please set it to the location containing the Qt version to build against. For example:"
        echo "  export QTDIR=/Applications/Qt/5.10.0/clang_64"
	exit 1
fi

QMAKE=${QTDIR}/bin/qmake
MAKE=make

# location of the source tree
SOURCEDIR=`pwd`'/src'

# Location to build PatternPaint
BUILDDIR=`pwd`'/build_mac'


################## Build JetiAppManager ###################
mkdir -p ${BUILDDIR}
pushd ${BUILDDIR}

${QMAKE} ${SOURCEDIR}/JetiAppManager.pro \
    -r \
    -spec macx-clang \
    CONFIG+=x86_64

#${MAKE} clean
${MAKE} -j6

popd


################## Package using macdeployqt #################
pushd ${BUILDDIR}

APP=${BUILDDIR}/JetiAppManager.app

# Integrate the system frameworks
${QTDIR}/bin/macdeployqt ${APP} -verbose=1


################## make DMG #################

DMG_NAME=JetiAppManager

mkdir ${DMG_NAME}
mv ${APP}/ ${DMG_NAME}/

hdiutil create -volname ${DMG_NAME} -srcfolder ${DMG_NAME} -ov -format UDZO ${DMG_NAME}.dmg

rm -R ${DMG_NAME}

popd

echo ""
echo "#####################################"
echo "JetiAppManager created successfully !"
echo ""
