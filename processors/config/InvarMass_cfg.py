'''
 * @file InvarMass_cfg.py
    * @ description here
    * @author Lisa Bouchard
    * @date 7 24 2020

'''
import HpstrConf
import sys

import baseConfig

parser = baseConfig.parser
# used to be (args, options) =, or something close to that.
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
invm  = HpstrConf.Processor('invm', 'InvarMassAnaProcessor')

###############################
#   Processor Configuration   #
###############################

# Sequence which the processors will run.

p.sequence = [invm]
#took out square brackets around input_file
p.input_files=input_file
p.output_files = [output_file]

if (options.nevents > -1):
    p.max_events = options.nevents
p.printProcess()