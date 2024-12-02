# -*- coding: utf-8 -*-
"""
Created on Wed Jan 24 17:59:25 2024

@author: Stage
"""

import math

HPAC_COEF_FILE = 'hpac_dispersion_coefs.csv'

INPUT_FOLDER = 'D:\\Results\\'

#RESULTS_FOLDER = 'C:\\Users\\stage\\Projects\\FastPlume\\Results\\'
RESULTS_FOLDER = 'D:\\Results\\'

RESULTS_FOLDER = 'D:\\Results\\temp'

TWO_PI = math.pi * 2

ROOT_TWO_PI = math.sqrt(TWO_PI)

'''
SPEED_FACTOR = 1.5
SIGX_FACTOR = 5
'''

'''
SPEED_FACTOR = 1
SIGX_FACTOR = 1
'''

'''
SPEED_FACTOR = 1.6
SIGX_FACTOR = 1 / SPEED_FACTOR * 1.5
'''

speed_factors = {
    'A' : 1.35,
    'B' : 1.47,   # est
    'C' : 1.58,   # est
    'D' : 1.7,
    'E' : 2.15,   # est
    'F' : 2.6,
    'G' : 2.6,   # est
    }

def get_speed_factor(stab):
    '''
    '''
    sf = speed_factors[stab]
    return sf


