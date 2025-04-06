
import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

os.chdir('./../') # Change from devtools/scripts directory to root

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])
subprocess.call([os.path.abspath("./Win-GenProjects.bat"), "nopause"])
