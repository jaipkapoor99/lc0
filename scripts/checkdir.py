#!/usr/bin/env python3

import os
import sys

if len(sys.argv) > 1 and os.path.isdir(sys.argv[1]):
    sys.exit(0)
sys.exit(1)
