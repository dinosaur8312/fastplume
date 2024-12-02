# -*- coding: utf-8 -*-
"""
Created on Wed Jan 24 18:22:00 2024

@author: Stage

See plume_tests.py for test routines.

"""

import os
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

from scipy.stats import norm

import config as cfg
from zfunction import zFunction
import utils
from parameter_dataframe import parameter_dataframe

def get_stab(istab):
    '''    
    '''
    stab = 'ABCDEFG'[istab]
    return stab

def get_istab(stab):
    '''    
    '''
    istab = 'ABCDEFG'.find(stab)
    return istab

def has_sigma0(df):
    '''
    True iff sig_x0, sig_y0 and sig_z0 are in df.
    '''
    ii = 0
    for c in ['x', 'y', 'z']:
        if not f'sig_{c}0' in df:
            break
        ii += 1
    return ii==3
    

def GetPlume(row):
    '''
    '''
    plume = Plume(row['Q_mg'], row['zplume'], row['dur'],
                  row['istab'], row['U'], row['zi'], row['title'])
    
    if has_sigma0(row):
        plume.sigma_model.calc_virtual_sources(row)
    
    return plume


class Plume():
    '''
    Class for computing concentrations and dosages from chemical plumes.
    '''

    def __init__(self, Q_mg=1.0e6, zplume=0, dur=0, istab=3, U=1, zi=10000, title=''):
        '''
        Constructor
        '''

        self.title = title

        self.set_release(Q_mg, zplume, dur=dur)

        fn = cfg.HPAC_COEF_FILE
        self.sigma_model = Sigma_Model(fn)
        self.set_met(istab, U, zi)

    def set_release(self, Q_mg, zplume, dur=0):
        '''
        '''
        self.Q_mg = Q_mg
        self.zplume = zplume
        self.dur = dur

    def set_met(self, istab, U, zi=10000):
        '''
        Set model coefficients
        to values for istab and U
        '''
        self.sigma_model.set_met(istab, U)
        self.zi = zi

    def set_loc(self, x, y, z, t):
        '''
        '''
        self.x = x
        self.y = y
        self.z = z

    def set_variables(self, values):
        '''
        '''
        for name, value in values.items():
            self.set_variable(name, value)

    def set_variable(self, name, value):
        '''
        '''
        if name in ['U', 'istab']:
            self.sigma_model.set_variable(name, value)

        else:
            self.__dict__['name'] = value

    def calc_sigmas(self, df):
        '''
        Input: df['x']

        Output:  df['sig_x', 'sig_y', 'sig_z']
        '''

        self.sigma_model.calc_sigmas(df['x'])
        for c in 'xyz':
            df.loc[:,f'sig_{c}'] = self.sigma_model.sigmas[f'sig_{c}']

    def calc_zFunction(self, df):
        '''
        Input: df['z', 'sig_z']

        Output:  df['zfunc']
        '''
        data = []

        for row in df.iloc:
            data.append(
                zFunction(row['z'], self.zplume, self.zi, row['sig_z']))

        df['zfunc'] = data

    def calc_concentration(self, df):
        '''
        Input: 
            df['x', 'y', 't']
            df[sig_x', 'sig_y', 'zfunc']

        Output: 
            df['yfunc', 'qyz']
            df[cpeak', 'conc']
        '''
        
        df['yfunc'] = norm.pdf(df['y'], scale=df['sig_y'])
        
        df['qyz'] = self.Q_mg * df['yfunc'] * df['zfunc']
        
        for c in ['cpeak', 'conc']:
            df[c] = 0
        
        U = self.Uplume
        
        # instantaneous releases
        bb0 = df['t'] >= 0
        bb0 &= df['dur'] < 1
        if sum(bb0)>0:
            
            df0 = df[bb0]
            
            arg = U * df0['t'] - df0['x']
            xfunc = norm.pdf(arg, scale=df0['sig_x'])
            
            conc = xfunc * df0['qyz']
            
            xfuncp = norm.pdf([min(x, 0) for x in df0['x']] , scale=df0['sig_x'])
            cpeak = xfuncp * df0['qyz']
            
            df.loc[bb0, 'cpeak'] = cpeak
            df.loc[bb0, 'conc'] = conc
            
        # semicontinuous releases
        bb1 = df['t'] >= 0
        bb1 &= df['dur'] >= 1
        if sum(bb1)>0:
            
            df1 = df[bb1]
        
            # tip
            tip = norm.cdf(df1['t'] * U - df1['x'], scale=df1['sig_x'])
            
            # tail
            ut = [max(0, t) * U for t in df1['t'] - df1['dur']]
            tail = norm.cdf(ut - df1['x'], scale=df1['sig_x'])
            
            conc = (df1['qyz'] / U) * (tip - tail) / df1['dur']
            
            
            '''
            arg = 0.5 * self.sigma_model.U * df1['dur'] / df1['sig_x']
            xfuncp = 1 / (self.sigma_model.U * df1['dur']) * (2 * norm.cdf(arg) - 1)
            # Correction for points having x<0
            xfuncp *= [utils.gauss(min(0,a)) for a in df1['x'] / df1['sig_x']]
            
            arg = self.sigma_model.U * tp / df1['sig_x']
            xfuncp = 1 / (self.sigma_model.U * df1['dur']) * (2 * norm.cdf(arg) - 1)            
            
            cpeak = df1['qyz'] * xfuncp
            '''
            
            # time of peak concentration            
            tp = np.array([self.tpeak(row['x'], row['U'] * self.sigma_model.SPEED_FACTOR, row['dur']) for row in df1.iloc])
            
            # tip
            tipp = norm.cdf(tp * self.Uplume - df1['x'], scale=df1['sig_x'])
            
            # tail
            utp = [max(0, t) * U for t in tp - df1['dur']]
            tailp = norm.cdf(utp - df1['x'], scale=df1['sig_x'])
            
            cpeak = (df1['qyz'] / U) * (tipp - tailp) / df1['dur']
            

            df.loc[bb1, 'cpeak'] = cpeak
            df.loc[bb1, 'conc'] = conc
            
            df.loc[bb1, 'ctip'] = tip
            df.loc[bb1, 'ctail'] = tail
            
    def tpeak(self, x, U, dur):
        '''
        Find time of peak concentration
        '''
        tp = max(dur, 0.5 * dur + x / U)
        return tp
            
        
    def calc_dosage(self, df):
        '''
        Input: 
            df['x', 'y', 't']
            df[sig_x', 'qyz']

        Output: 
            df['dinf', 'dose']
        '''

        # all-times dosage
        #df['dinf'] = df['qyz'] / self.sigma_model.U
        
        for c in ['dose', 'dinf']:
            df[c] = 0
        
        # instantaneous releases
        bb0 = df['t'] >= 0
        bb0 &= df['dur'] < 1
        if sum(bb0)>0:
            
            df0 = df[bb0]            
            
            arg0 = - df0['x']
            arg1 = self.Uplume * df0['t'] - df0['x']
            
            xfunc = (norm.cdf(arg1, scale=df0['sig_x']) - norm.cdf(arg0, scale=df0['sig_x'])) / self.sigma_model.Uplume
            
            dose = xfunc * df0['qyz']
            
            xfuncinf = (1 - norm.cdf(arg0, scale=df0['sig_x'])) / self.sigma_model.Uplume
            
            df.loc[bb0, 'dose']  = dose
            
            df.loc[bb0, 'dinf'] = xfuncinf * df0['qyz']
            
        # semicontinuos releases
        bb1 = df['t'] >= 0
        bb1 &= df['dur'] >= 1
        if sum(bb1)>>0:
            
            df1 = df[bb1]            
            
            # tip
            tip1 = utils.Icdf(self.sigma_model.Uplume * df1['t'] - df1['x'], scale=df1['sig_x'])
            tip0 = utils.Icdf(- df1['x'], scale=df1['sig_x'])
            tip = tip1 - tip0
            
            # tail
            # For t<=T, tail is attached to source
            tail0 = norm.cdf(-df1['x'], scale=df1['sig_x']) * [min(row['t'], row['dur']) for row in df1.iloc] * self.sigma_model.U / df1['sig_x']
            
            # For t>= T, tail is moving
            arg = [self.Uplume * max(0, t) for t in df1['t'] - df1['dur']]
            tail2 = utils.Icdf( arg - df1['x'], scale=df1['sig_x'])
            tail1 = utils.Icdf(- df1['x'], scale=df1['sig_x'])
            
            tail = tail2 - tail1 + tail0 

            const = df1['qyz'] * df1['sig_x'] / (df1['dur'] * (self.sigma_model.Uplume*self.sigma_model.Uplume))
            dose = (tip - tail) * const
            df.loc[bb1, 'dose']  = dose
            
            dinf = df1['qyz'] / self.sigma_model.Uplume
            #df.loc[bb1, 'dinf0'] = dinf
            
            dinf *= norm.cdf(df1['x'], scale=df1['sig_x'])
            df.loc[bb1, 'dinf'] = dinf
            
            '''
            df.loc[bb1, 'tip0'] = tip0
            df.loc[bb1, 'tip1'] = tip1
            df.loc[bb1, 'tip'] = tip
            df.loc[bb1, 'tail2'] = tail2
            df.loc[bb1, 'tail1'] = tail1
            df.loc[bb1, 'tail0'] = tail0
            df.loc[bb1, 'tail'] = tail
            '''
            return
            
    def calc_dosages(self, df):
        '''
        Calculate all dosages.

        Input: df['x', 'y', 'z', 't', ]

        Output:  
            df['sig_x', 'sig_y', 'sig_z', 'xfunct', 'qyz'],
            df['cpeak', 'conc']
            df['dinf', 'dose']
        '''
        if has_sigma0(df):
            self.sigma_model.calc_virtual_sources(df)
        
        self.calc_sigmas(df)
        self.calc_zFunction(df)
        self.calc_concentration(df)
        self.calc_dosage(df)


# End of class Plume
# -----------------------------------------------------

class Sigma_Model():
    '''
    Class for calculating dispersion coefficients.
    '''

    def __init__(self, fname='', istab=None, U=None):

        self.fname = fname

        if len(fname) > 0:
            self.read_model_coef(fname)

        if istab != None and U != None:
            self.set_met(istab, U)

        self.point_source()

    def read_model_coef(self, fname):
        '''
        Read a model coefficient file
        and return a pivot table.

        The index of the pivot table is [istab, wind, x]
        '''

        mc = pd.read_csv(fname)
                
        mc1 = mc[mc['how'] == 'fit']
        self.coefs = mc1
        
        self.istabs = sorted(mc1['istab'].unique())
        self.speeds = sorted(mc1['wind'].unique())
        self.dists = sorted(mc1['x'].unique())

        pivot = pd.pivot_table(mc1, values=['sig_x', 'sig_y', 'sig_z'], index=[
                               'istab', 'wind', 'x'])  # , columns=['sigs'])

        pivot['ln_x'] = list(map(math.log, pivot['sig_x']))
        for c in ['sig_x', 'sig_y', 'sig_z']:
            pivot[f'ln_{c}'] = list(map(math.log, pivot[c]))

        self.pivot = pivot
        
    def get_stab(self, istab):
        '''
        Return the stability class (A-G) corresponding to istab.
        '''
        stab = 'ABCDEFG'[istab]
        return stab
    
    def get_istab(self, stab):
        '''
        Return istab for for the given stability class (A-G).
        '''
        istab = 'ABCDEFG'.find(stab)
        return istab
    
    def get_iwind(self, U):
        '''
        Return iwind corresponding to the specified wind speed.
        speeds[iwind] <= U.
        '''
        spd = pd.Series([0]+self.speeds).unique()
        ii = [i for i in range(0,len(spd)) if spd[i] <= U]
        if len(ii) <1:
            iwind = 0
        else:
            iwind = max(ii)
        
        return iwind

    def is_valid_combination(self, istab, U):
        '''
        Return True if istab, U is stability, wind speed combination in the coefficient data.
        '''
        bb = (self.coefs['istab'] == istab) & (self.coefs['wind'] == U)

        result = sum(bb) > 0
        return result
    
    def get_valid_combinations(self):
        '''
        Return all valid combinations of istab, U.
        '''
        
        spd = pd.Series([0]+self.speeds).unique()
        data = []
        for istab in range(0,7):
            for iwind in range(0, len(spd)):
                U = spd[iwind]
                if self.is_valid_combination(istab, U):
                    data.append([istab, iwind, U])
        df = pd.DataFrame(data, columns=['istab', 'iwind', 'U'])
        return df

    def set_variable(self, name, value):
        '''
        '''
        if name in ['U', 'istab']:
            self.set_variable(name, value)

        else:
            self.__dict__['name'] = value

    def set_met(self, istab, U):
        '''
        Set stability and wind speed

        The interpolation used here is linear in both speed and sigmas.

        This algorithm requires that same distances be used
        for all stabillities and wind speeds in the coefficient data.
        '''
        self.istab = istab
        self.U = U
        
        self.SPEED_FACTOR = cfg.get_speed_factor(get_stab(istab))
        
        # Transport speed of the plume.
        # Is calibrate4d to give results that match HPAC.
        self.Uplume = self.U * self.SPEED_FACTOR

        # Speeds for this stability
        speeds = pd.Series([indx[0]
                           for indx in self.pivot.loc[istab].index]).unique()
        
        if U <= min(speeds):
            piv = self.pivot.loc[istab, min(speeds)]
        elif U >= max(speeds):
            piv = self.pivot.loc[istab, max(speeds)]
        else:
            U0 = max([u for u in speeds if u <= U])
            U1 = min([u for u in speeds if u > U])
            piv0 = self.pivot.loc[istab, U0]
            piv1 = self.pivot.loc[istab, U1]

            rr = (U - U0) / (U1 - U0)

            piv = pd.DataFrame()
            for s in ['sig_x', 'sig_y', 'sig_z']:
                t = f'ln_{s}'
                piv[t] = (1-rr) * piv0[t] + rr * piv1[t]
                piv[s] = list(map(math.exp, piv[t]))

        piv['ln_x'] = list(map(math.log, list(piv.index)))

        self.piv = piv

    def calc_sigmas(self, xx):
        '''
        Calculate sigma_x, sigma_y, and sigma_z.
        
        xx is downwind distances (must be iterable).

        The interpolation used here is logarithmic in both x and sigmas.
        '''
        piv = self.piv

        # Use source sigmas for points upwind of the source
        xx0 = [max(0, x) for x in xx]

        xmin = min(piv.index)
        xmax = max(piv.index)

        sigmas = pd.DataFrame()
        sigmas['x'] = xx
        
        for xv_row in self.x_virt.iloc:
            
            for c in 'xyz':
    
                # Virtual source
                xv = xv_row[f'xv_{c}']
    
                # Add virtual source and clip to range in sigma table.
                xx1 = [min(xmax, max(xmin, x + xv)) for x in xx0]
                log_xx = list(map(math.log, xx1))
    
                sigmas[f'sig_{c}'] = [math.exp(s) for s in np.interp(
                    log_xx, piv['ln_x'], piv[f'ln_sig_{c}'])]
    
        self.sigmas = sigmas

        # xx = [-1, -2, 0, 2, 3, 4, 99900.0, 100100.0, 100200.0]

    def point_source(self):
        '''
        Set source sigmas to 0. 
        '''
        cols = ['sig_x0', 'sig_y0', 'sig_z0', 'xv_x', 'xv_y', 'xv_z']
        x_virt = pd.DataFrame([len(cols)*[0]], columns=cols)
        self.x_virt = x_virt

    def calc_virtual_sources(self, sigma0):
        '''
        Calculate virtual sources from source sigmas.

        The interpolation used here is logarithmic in both x and sigmas.
        
        Computes self.x_virt : pandas.DataFrame
        
        sigma0 can be type: list, dictionary, pandas.Series, or pandas.DataFrame.
        If sigma0 is a dictionary or pandas.Series, v_virt will have one row.
        If sigma0 is a pandas.DataFrame, v_virt will have the same number of rows as sigma0
        '''
        piv = self.piv
        
        # Create x_virt, which is a DataFrame containing the source sigmas
        if type(sigma0) == list:
            x_virt = pd.DataFrame([sigma0], columns=['sig_x0', 'sig_y0', 'sig_z0'])
        elif type(sigma0) == dict:
            x_virt = pd.DataFrame(pd.Series(sigma0)).transpose()
        elif type(sigma0) == pd.core.frame.Series:
            x_virt = pd.DataFrame(sigma0[['sig_x0', 'sig_y0', 'sig_z0']]).transpose()
        elif type(sigma0) == pd.core.frame.DataFrame:
            x_virt = sigma0[['sig_x0', 'sig_y0', 'sig_z0']].copy()
        else:
            # We really shouldn't get here.
            x_virt = pd.DataFrame(sigma0[['sig_x0', 'sig_y0', 'sig_z0']]).transpose()
        #x_virt.columns = ['sig_x0', 'sig_y0', 'sig_z0']
        
        for c in 'xyz':
            sigmin = min(piv[f'sig_{c}'])
            sigmax = max(piv[f'sig_{c}'])
            ln_sig = [math.log(min(max(sigmin, s), sigmax))
                      for s in x_virt[f'sig_{c}0']]
            x_virt[f'xv_{c}'] = [math.exp(s) for s in np.interp(
                ln_sig, piv[f'ln_sig_{c}'],  piv['ln_x'])]

        self.x_virt = x_virt

# End of class Sigma_Model
# -----------------------------------------------------

if __name__ == '__main__':

    pass

    #path = '.'
    #fn = 'hpac_dispersion_coefs.csv'
    #fname = os.path.join(path, fn)
    
    #status, result = test_virtual_sources_sub_01(4, 6.7)
    
    if False:
        Q_mg, zplume, dur = 1.0e6, 0, 0
        istab, U, zi = 3, 1, 100000
        title = 'test'

        plume = Plume(Q_mg, zplume, dur, istab, U, zi, title)

        xx = [10, 100, 1000]

        zrcp = 0

        df = pd.DataFrame()
        df['x'] = xx
        df['y'] = 0
        df['z'] = zrcp
        df['t'] = 100
        df['dur'] = 0

        plume.calc_sigmas(df)

        #sigmas = plume.sigma_model.sigmas

        plume.calc_zFunction(df)
        #plume.calc_peak_concentration(df)
        #plume.calc_dosage_inf(df)

        plume.calc_concentration(df)
        plume.calc_dosage(df)
        df

    if False:
        
        fname = cfg.HPAC_COEF_FILE
        
        istab, U = 3, 5
        sigma_model = Sigma_Model(fname, istab, U)

        piv = sigma_model.piv
        x_virt = sigma_model.x_virt

        data = [
            [10, 100, 10],
            [20, 50, 20],
            [25, 25, 5],
            [25, 25, 0],
            [25, .01, 5],
            [125000, 25, 10]
        ]
        data = [
            [10, 100, 10],
        ]
        sig0 = pd.DataFrame(data, columns=['sig_x0', 'sig_y0', 'sig_z0'])

        sigma_model. calc_virtual_sources(sig0)
        x_virt = sigma_model.x_virt
        print(x_virt)

        xx = [-100, -10, 0] + list(np.logspace(-1, 3, 9))

        result_list = []
        for s0 in sig0.iloc:
            result = pd.DataFrame()
            result['istab'] = len(xx) * [istab]
            result['speed'] = len(xx) * [U]
            result['stab'] = len(xx) * ['ABCDEFG'[istab]]

            #sigma_model.calc_virtual_sources(pd.DataFrame(s0))
            sigma_model.calc_virtual_sources(s0)
            sigma_model.calc_sigmas(xx)
            result[sigma_model.sigmas.columns] = sigma_model.sigmas[sigma_model.sigmas.columns]

            result_list.append(result)

        #qq = list(map(math.log, list(piv.index)))

    if False:
        
        fname = cfg.HPAC_COEF_FILE
        sigma_model = Sigma_Model(fname)

        pivot = sigma_model.pivot
        istabs = sigma_model.istabs
        speeds = sigma_model.speeds
        dists = sigma_model.dists

        # pivot.loc[4,:]

        # pivot.loc[:,speeds[2]]

        pivot[pivot['istab'] == 1]

        pivot.loc[1, 2.3]
        pivot.loc[1, 2.3, 10.0]
        pivot.loc[1, 2.3, 10.0:100.0]
        pivot.loc[1:2, 2.3, 10.0:100.0]

        pivot.loc[1:2, 2.3, 9.0:100.0]
        '''
        pivot = read_model_coef_pivot(fname)
        
        Q_mg, istab, U =  1.0e6, 4, 5
        
        plume = Plume(model_coefs, Q_mg, istab, U)
        
        model_coefs.loc[istab, 6.7]
        
        
        speeds = sorted(pd.Series([i[1] for i in model_coefs.index]).unique())
    
        path = '.'
        fn = 'fit_param_tab_all.csv'
        fname = os.path.join(path, fn)
        mc = pd.read_csv(fname)
        mc1 = mc[mc['variable']=='sig_y'].copy()
        mc1['variable'] = 'sig_x'
        
        mc2 = pd.concat([mc,mc1])
        
        '''
