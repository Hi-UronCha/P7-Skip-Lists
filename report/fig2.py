import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import io

# 1. 嵌入数据
csv_data = """N,MemoryMB
1000,0.0385
2000,0.0757
5000,0.1908
10000,0.3815
20000,0.7610
50000,1.9084
100000,3.8181
200000,7.6319
500000,19.0721
"""

# 2. 读取数据
df = pd.read_csv(io.StringIO(csv_data))

# --- 绘图设置 ---
plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['axes.unicode_minus'] = False
try:
    plt.style.use('ggplot')
except:
    plt.grid(True, which="both", ls="-")

# 3. 计算双对数下的拟合 (Log-Log Fit)
# 公式: log(y) = b * log(x) + log(a)
# 其中 b 是斜率 (Slope)，如果 b ≈ 1，则证明是 O(N)
log_x = np.log10(df['N'])
log_y = np.log10(df['MemoryMB'])

slope, intercept = np.polyfit(log_x, log_y, 1) # 1次多项式拟合

# 计算 R^2
y_pred_log = slope * log_x + intercept
r_squared = 1 - (sum((log_y - y_pred_log)**2) / ((len(log_y) - 1) * np.var(log_y, ddof=1)))

# 4. 绘图
plt.figure(figsize=(10, 6))

# 绘制原始数据点
plt.scatter(df['N'], df['MemoryMB'], color='purple', s=80, alpha=0.8, label='Actual Memory Usage')

# 绘制拟合线
# 为了在 Log 轴上画出直线，我们需要生成一系列密集的点
x_fit = np.logspace(np.log10(df['N'].min()), np.log10(df['N'].max()), 100)
y_fit = (10**intercept) * (x_fit**slope) # 还原回原始坐标系的值
plt.plot(x_fit, y_fit, "r--", linewidth=2, label=f'Power Law Fit ($y \\propto x^{{{slope:.4f}}}$)')

# 5. 设置为双对数坐标
plt.xscale('log')
plt.yscale('log')

# 6. 美化与标注
plt.title('SkipList Space Complexity (Log-Log Scale)', fontsize=14)
plt.xlabel('Number of Elements ($N$) - Log Scale', fontsize=12)
plt.ylabel('Memory Usage (MB) - Log Scale', fontsize=12)

# 标注斜率的意义
info_text = (f"Log-Log Slope = {slope:.4f}\n"
             f"$R^2 = {r_squared:.4f}$\n"
             f"Slope $\\approx$ 1.0 implies $O(N)$")
             
plt.text(df['N'].min(), df['MemoryMB'].max(), 
         info_text, 
         fontsize=12, 
         verticalalignment='top',
         bbox=dict(facecolor='white', alpha=0.9, edgecolor='gray', boxstyle='round'))

plt.grid(True, which="both", ls="-", alpha=0.5) # 开启更密集的网格
plt.legend(fontsize=11)
plt.tight_layout()

# 保存
plt.savefig('skiplist_space_loglog.png', dpi=300)
print("Saved: skiplist_space_loglog.png")
# plt.show()