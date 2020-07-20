'''
 * @file TriggerAna_cfg.py
    * @ description here
    * @author Lisa Bouchard
    * @date 6 12 2020
    * Using mcTuple_cfg.py as example code
'''
import HpstrConf
import sys

import baseConfig

parser = baseConfig.parser
options = parser.parse_args()
# Use the input file to set the output file name

input_file = options.inFilename
output_file = options.outFilename

print('Input ROOT file: %s' % input_file)
print('Output ROOT file: %s' % output_file)

p = HpstrConf.Process()

p.run_mode = 1
#p.max_events = 1000

# Library containing processors
p.add_library("libprocessors")

###############################
#          Processors         #
###############################
ts  = HpstrConf.Processor('trg', 'TriggerAnaProcessor')

###############################
#   Processor Configuration   #
###############################

# Sequence which the processors will run.

p.sequence = [ts]

p.input_files=input_file
p.output_files = [output_file]

if (options.nevents > -1):
    p.max_events = options.nevents
p.printProcess()