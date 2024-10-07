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

# Main function to run the code
def main():
    # Specify the path to your CSV file
    file_path = 'hpac_dispersion_coefs.csv'

    # Run the preprocessing function
    processed_df, ln_x_fit_params = preprocess_data(file_path)

    # Output the processed data (display or save)
    print("Processed Data for ln_sig_x, ln_sig_y, ln_sig_z (per istab, wind):")
    print(processed_df)

    print("\nLeast Squares Parameters for ln_x (global across all istab, wind):")
    print(f"Slope: {ln_x_fit_params[0]}, Intercept: {ln_x_fit_params[1]}")

    # Optionally, save the preprocessed data to a new CSV file
    output_file = 'processed_coefficients.csv'
    processed_df.to_csv(output_file, index=False)
    print(f"\nProcessed data saved to {output_file}")

if __name__ == '__main__':
    main()
