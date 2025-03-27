import numpy as np
import scipy.signal as signal
import csv


order = 2  # filter order (Keep in mind, the filter code in "Filters.h must also be changed")
f_start = 100  # Start frequency (Hz)
f_end = 2000  # End frequency (Hz)
num_subdivisions = 10  # Number of frequency subdivisions
sampling_rate = 4096  # Sampling rate (Hz)
Q_factor = 2 ** 14# Q15 ( 14, due to >1 Coefficients )

frequencies = np.logspace(np.log10(f_start), np.log10(f_end), num=num_subdivisions)

csv_data = []

csv_data.append(['Frequency (Hz)', 'b_0', 'b_1', 'b_2', 'a_0', 'a_1', 'a_2'])

for f in frequencies:
    # Normalize the cutoff frequency for filter
    wn = f / (0.5 * sampling_rate)  # Normalized cutoff frequency
    b, a =  signal.butter(order, wn, btype='low', analog=False)
    a = np.floor(Q_factor*a+.5) #.5 for rounding
    b = np.floor(Q_factor*b+.5)
    csv_data.append([(f), int(b[0]),int(b[1]),int(b[2]),int(a[0]),int(a[1]),int(a[2])])

csv_filename = 'filter_coefficients.csv'
with open(csv_filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(csv_data)

print(f"CSV file '{csv_filename}' has been written with the filter coefficients.")