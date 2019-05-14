# Tunguska by Team210 - 64k intro by Team210 at Solskogen 2k19
# Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import os
import argparse

# Get available symbols
symbol_files = os.listdir("symbols")

# Remove all files that do not have ".frag" ending
symbol_files = [ f for f in symbol_files if f.endswith('.frag') ] 
symbol_names = [ f.replace('.frag', '') for f in symbol_files ]

# Read all symbol code from symbol files
symbol_codes = []
for symbol_file in symbol_files:
    symbol_code = ""
    with open("symbols/"+symbol_file, "rt") as f:
        symbol_code = f.read()
        f.close()
    symbol_codes += [ symbol_code ]

# Parse command line args
parser = argparse.ArgumentParser(description='Team210 symbol packer.')
parser.add_argument('-o', '--output', dest='out')
args, rest = parser.parse_known_args()

if rest == []:
    print("Error: No input files present.")
    exit()
    
# Generate header file from all shaders
for inputfile in rest:
    input_source_lines = None
    
    with open(inputfile, "rt") as f:
        input_source_lines = f.readlines()
        f.close()
    
    # Extract symbol list from source file
    symbol_list = []
    for line in input_source_lines:
        if 'void' in line and ';' in line:
            print(line.split()[1].split('(')[0])
    
