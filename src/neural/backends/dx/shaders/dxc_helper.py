#!/usr/bin/env python
# Call dxc with absolute paths where '/' are converted to '\' (in windows).
import os
import re
import subprocess
import sys

x = sys.argv[1:]
for i in range(len(x)):
    if re.match(r"[a-zA-Z]:/", x[i]):
        x[i] = os.path.normpath(x[i])
# We assume dxc is already on the path.
sys.exit(subprocess.call(["dxc"] + x))
