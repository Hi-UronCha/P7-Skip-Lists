import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import io

# 1. 嵌入数据
csv_data = """N,InsertTime,SearchTime
50000,0.018333,0.015333
100000,0.048667,0.048667
150000,0.103667,0.157333
200000,0.137333,0.234667
250000,0.185333,0.307667
300000,0.265667,0.469333
350000,0.359333,0.600000
400000,0.507333,0.789667
450000,0.657000,1.053333
500000,0.848667,1.258667
550000,0.904000,1.398667
600000,1.179667,1.652667
650000,1.500000,2.107333
700000,1.356333,1.943000
750000,1.639333,2.274667
800000,1.626667,2.427333
850000,1.909333,2.594667
900000,2.338333,2.946333
950000,2.326333,3.247667
1000000,2.702333,3.311000
1050000,2.969000,3.728667
1100000,3.033333,3.666333
1150000,2.886333,3.914000
1200000,3.353333,4.338333
1250000,3.534333,4.573000
1300000,3.729333,5.152667
1350000,3.654333,4.796000
1400000,3.540667,4.755000
1450000,3.686333,4.957667
1500000,4.349000,5.420333
1550000,4.471667,5.506000
1600000,4.618000,6.249000
1650000,5.415333,7.083667
1700000,5.752667,7.431667
1750000,5.531333,6.904667
1800000,6.733000,8.289000
1850000,7.001000,8.079333
1900000,6.164333,8.068333
1950000,6.578667,8.360333
2000000,6.652000,8.438667
"""

# 2. 读取数据并变换
df = pd.read_csv(io.StringIO(csv_data))
df['x_transformed'] = df['N'] * np.log2(df['N'])

# --- 样式设置 ---
plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['axes.unicode_minus'] = False
try:
    plt.style.use('ggplot')
except:
    plt.grid(True)

# --- 辅助函数：分段拟合与绘图 ---
def plot_segmented_fit(ax, df_data, y_col_name, title):
    x_total = df_data['x_transformed']
    y_total = df_data[y_col_name]
    
    # 绘制原始散点
    ax.scatter(x_total, y_total, color='gray', alpha=0.5, label='Actual Data', s=40)
    
    # 定义分段阈值 (根据数据点数量大概三等分，同时考虑物理缓存层级)
    # Segment 1: N <= 550,000 (Fits in fast cache)
    # Segment 2: 550,000 < N <= 1,350,000 (Transition)
    # Segment 3: N > 1,350,000 (RAM limited)
    
    limit1 = 550000
    limit2 = 1350000
    
    # 划分数据 (使用 >= 和 <= 制造一点重叠，保证线是视觉连续的)
    seg1 = df_data[df_data['N'] <= limit1]
    seg2 = df_data[(df_data['N'] >= limit1) & (df_data['N'] <= limit2)]
    seg3 = df_data[df_data['N'] >= limit2]
    
    segments = [
        (seg1, 'green', 'Stage 1 (Small N)'),
        (seg2, 'orange', 'Stage 2 (Mid N)'),
        (seg3, 'red',   'Stage 3 (Large N)')
    ]
    
    for df_seg, color, label_base in segments:
        x_seg = df_seg['x_transformed']
        y_seg = df_seg[y_col_name]
        
        # 线性拟合
        z = np.polyfit(x_seg, y_seg, 1) # z[0] is slope, z[1] is intercept
        p = np.poly1d(z)
        
        # 计算 R^2
        y_pred = p(x_seg)
        # 防止除零错误（虽然这里数据量大不会）
        if len(y_seg) > 1:
            r2 = 1 - (sum((y_seg - y_pred)**2) / ((len(y_seg) - 1) * np.var(y_seg, ddof=1)))
        else:
            r2 = 1.0
            
        # 绘制实线 (linestyle='-')
        # 斜率 z[0] 代表单位计算量的耗时，越大约慢
        label_text = f"{label_base}: Slope={z[0]*1e7:.2f}, $R^2$={r2:.3f}"
        ax.plot(x_seg, y_pred, color=color, linestyle='-', linewidth=2.5, label=label_text)

    ax.set_title(title, fontsize=14)
    ax.set_xlabel('Theoretical Complexity ($N \cdot \log_2 N$)', fontsize=12)
    ax.set_ylabel('Total Time (seconds)', fontsize=12)
    ax.legend(fontsize=10, loc='upper left')

# ==========================================
# 绘图逻辑
# ==========================================

# 1. Insert Fit
fig1, ax1 = plt.subplots(figsize=(10, 6))
plot_segmented_fit(ax1, df, 'InsertTime', 'SkipList Insertion: 3-Stage Segmented Fit')
plt.tight_layout()
plt.savefig('skiplist_insert_segmented.png', dpi=300)
print("Saved: skiplist_insert_segmented.png")

# 2. Search Fit
fig2, ax2 = plt.subplots(figsize=(10, 6))
plot_segmented_fit(ax2, df, 'SearchTime', 'SkipList Search: 3-Stage Segmented Fit')
plt.tight_layout()
plt.savefig('skiplist_search_segmented.png', dpi=300)
print("Saved: skiplist_search_segmented.png")

# plt.show()