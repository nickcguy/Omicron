import os
import shutil
import glob
from distutils.dir_util import copy_tree

# Package configuration

targetDir="Build"
buildDir="cmake-build-relwithdebinfo"
assetDir="_assets"
contentDir="_content"
runtimeDir="_runtime"
exeName="OmicronRender.exe"
runDir="_Working"

### END Package configuration

# shutil.rmtree(targetDir)

if not os.path.exists(targetDir):
    os.makedirs(targetDir)

shutil.copy2(buildDir+"/"+exeName, targetDir+"/")
copy_tree(assetDir, targetDir+"/assets")
copy_tree(contentDir, targetDir)
copy_tree(runtimeDir, targetDir)

copy_tree(targetDir, runDir)