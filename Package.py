import os
import shutil
from distutils.dir_util import copy_tree

# Package configuration

targetDir="Build"
buildDir="cmake-build-relwithdebinfo"
assetDir="_assets"
contentDir="_content"
runtimeDir="_runtime"
exeName="OmicronRender.exe"

### END Package configuration

if not os.path.exists(targetDir):
    os.makedirs(targetDir)

shutil.copy2(buildDir+"/"+exeName, targetDir+"/")
copy_tree(assetDir, targetDir+"/assets")
copy_tree(contentDir, targetDir)
copy_tree(runtimeDir, targetDir)