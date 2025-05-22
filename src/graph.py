import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Load the data
df = pd.read_csv(
    '/home/cryptic/projetoda/Delivery-Truck-Pallet-Packing-Optimization/src/benchmark.csv'
)

# Pivot so that rows = Dataset, columns = Algorithm, values = Time(sec)
pivot = df.pivot(index='Dataset', columns='Algorithm', values='Time(sec)')

# Create a mask of timeouts (we used -1.0 for any run >2s)
timeout_mask = pivot <= 0

# Replace timeouts with NaN so they don't plot
plot_data = pivot.mask(timeout_mask, np.nan)

# --- PLOT ---
fig, ax = plt.subplots(figsize=(12, 6))

# Grouped bar chart of only the successful runs
bar_container = plot_data.plot(
    kind='bar',
    ax=ax,
    width=0.8,        # total bar-group width
    logy=True,        # log scale y-axis
    legend=True,
    rot=0             # keep dataset labels horizontal
)

plt.title('Algorithm Performance Comparison')
plt.xlabel('Dataset Number')
plt.ylabel('Execution Time (seconds, log scale)')

# Tweak legend
plt.legend(
    title='Algorithm',
    bbox_to_anchor=(1.05, 1),
    loc='upper left'
)

# Draw horizontal grid lines only
plt.grid(axis='y', linestyle='--', alpha=0.7)

# --- Annotate Timeouts ---
# Find a small y-coordinate below the smallest real bar
min_positive = plot_data.min().min()
# If all runs succeeded on tiny datasets, avoid zero:
y_text = min_positive * 0.5 if min_positive > 0 else 1e-3

# Compute bar positions
n_datasets, n_algos = plot_data.shape
bar_width = 0.8 / n_algos
x = np.arange(n_datasets)

for i, algo in enumerate(plot_data.columns):
    # x-coordinate for this algorithm's bars
    x_positions = x - 0.8/2 + i * bar_width + bar_width/2
    for xi, ds in enumerate(plot_data.index):
        if timeout_mask.iloc[xi, i]:
            # Place “TO” at y_text
            ax.text(
                x_positions[xi], y_text,
                '∅',
                ha='center', va='bottom',
                fontsize='small',
                rotation=0
            )

plt.tight_layout()
plt.savefig('benchmark_grouped_bar.png')
plt.show()
