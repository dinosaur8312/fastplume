import sys
import os

# Add the 'build' directory to sys.path
build_dir = os.path.join(os.path.dirname(__file__), 'build')
sys.path.append(build_dir)

# Now import the cpp_engine module from the build directory
import fp_cpp_engine  # This will be the name of the pybind11 module

import pandas as pd
import numpy as np
from scipy import stats

# Function to read the CSV and create the pivot table
def read_model_coef(file_path):
    '''
    Read a model coefficient file and return a pivot table.
    The index of the pivot table is [istab, wind, x].
    '''
    mc = pd.read_csv(file_path)
    mc1 = mc[mc['how'] == 'fit']

    # Create a pivot table with index [istab, wind, x] and columns [sig_x, sig_y, sig_z]
    pivot = pd.pivot_table(mc1, values=['sig_x', 'sig_y', 'sig_z'], index=['istab', 'wind', 'x'])

    # Calculate ln(sig_x), ln(sig_y), ln(sig_z)
    for c in ['sig_x', 'sig_y', 'sig_z']:
        pivot[f'ln_{c}'] = np.log(pivot[c])

    return pivot

# Function to check if all x vectors are the same for all (istab, wind)
def check_x_uniformity(pivot):
    # Extract unique (istab, wind) groups and corresponding x values
    istab_wind_groups = pivot.groupby(level=['istab', 'wind'])
    
    # Get the first x vector for comparison
    first_group = next(iter(istab_wind_groups))[1]
    first_x = np.log(first_group.index.get_level_values('x'))

    # Check if all (istab, wind) groups have the same x values
    for (istab, wind), group in istab_wind_groups:
        current_x = np.log(group.index.get_level_values('x'))
        if not np.array_equal(first_x, current_x):
            return False, None
    
    return True, first_x

# Step 1: Process the pivot table for least squares fitting
def preprocess_data(file_path):
    # Read the pivot table
    pivot = read_model_coef(file_path)

    # Check if all ln(x) vectors are the same
    uniform_x, ln_x = check_x_uniformity(pivot)

    if uniform_x:
        # Perform least squares fitting on ln_x with indices once for the whole dataset
        indices = np.arange(len(ln_x))
        slope_ln_x, intercept_ln_x, _, _, _ = stats.linregress(indices, ln_x)
        ln_x_fit_params = [slope_ln_x, intercept_ln_x]
    else:
        raise ValueError("x vectors are not uniform across istab, wind groups.")

    least_square_coeffs = []

    # Get unique istab and wind combinations from the pivot table
    istab_wind_groups = pivot.groupby(level=['istab', 'wind'])

    # Loop through each (istab, wind) group
    for (istab, wind), group in istab_wind_groups:
        # Sort by ln_x for consistency
        group_sorted = group.sort_values('ln_sig_x')

        # Get the indices for least squares fitting
        indices = np.arange(len(group_sorted))

        # Perform least squares fitting between index and ln_sig_x, ln_sig_y, ln_sig_z
        slope_sig_x, intercept_sig_x, _, _, _ = stats.linregress(indices, group_sorted['ln_sig_x'])
        slope_sig_y, intercept_sig_y, _, _, _ = stats.linregress(indices, group_sorted['ln_sig_y'])
        slope_sig_z, intercept_sig_z, _, _, _ = stats.linregress(indices, group_sorted['ln_sig_z'])

        # Store the least squares coefficients for ln_sig_x, ln_sig_y, ln_sig_z
        least_square_coeffs.append([istab, wind, slope_sig_x, intercept_sig_x, 
                                    slope_sig_y, intercept_sig_y, 
                                    slope_sig_z, intercept_sig_z])

    # Convert the results into a DataFrame for easier handling
    columns = ['istab', 'wind', 'slope_sig_x', 'intercept_sig_x', 'slope_sig_y', 
               'intercept_sig_y', 'slope_sig_z', 'intercept_sig_z']
    processed_df = pd.DataFrame(least_square_coeffs, columns=columns)

    return processed_df, ln_x_fit_params

def send_data_to_cpp(processed_df, ln_x_fit_params):
    # Create an instance of the C++ taskData class using float precision
    dispersion_data = cpp_engine.taskData()

    # Loop over the DataFrame rows and add data to the C++ class
    for _, row in processed_df.iterrows():
        istab = int(row['istab'])
        wind = float(row['wind'])

        # Create an instance of IstabWindData using float precision
        wind_data = cpp_engine.IstabWindData(wind)

        # Add the coefficients
        wind_data.slope_sig_x = float(row['slope_sig_x'])
        wind_data.intercept_sig_x = float(row['intercept_sig_x'])
        wind_data.slope_sig_y = float(row['slope_sig_y'])
        wind_data.intercept_sig_y = float(row['intercept_sig_y'])
        wind_data.slope_sig_z = float(row['slope_sig_z'])
        wind_data.intercept_sig_z = float(row['intercept_sig_z'])

        # Add the wind data to the C++ taskData class
        dispersion_data.add_istab_wind_data(istab, wind_data)

    # Add global ln_x coefficients to the C++ class
    dispersion_data.set_ln_x_fit_params(float(ln_x_fit_params[0]), float(ln_x_fit_params[1]))

    # Optionally print the data from C++ for verification
    dispersion_data.print_data()

# Main function to run the code
def main():
    # Specify the path to your CSV file
    file_path = 'hpac_dispersion_coefs.csv'

    # Run the preprocessing function
    processed_df, ln_x_fit_params = preprocess_data(file_path)

    # Send the preprocessed data to C++
    send_data_to_cpp(processed_df, ln_x_fit_params)

if __name__ == '__main__':
    main()
