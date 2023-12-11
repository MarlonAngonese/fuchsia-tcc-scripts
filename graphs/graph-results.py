import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Carregando os dados
data = pd.read_csv('./results.csv')

# Adicionando uma coluna para identificar os casos de teste
data['Test Case'] = data.index % 7 + 1

# Configurando o estilo dos gráficos
sns.set(style="whitegrid")

# Gráfico de Latência para cada caso de teste
plt.figure(figsize=(10, 6))
sns.barplot(x='Test Case', y='Execution Time Mean', hue='Scheduler', data=data)
plt.title('Média de Latência para Cada Caso de Teste')
plt.ylabel('Latência Média (ms)')
plt.xlabel('Caso de Teste')
plt.show()

# Gráfico de Tempo de Espera para cada caso de teste
plt.figure(figsize=(10, 6))
sns.barplot(x='Test Case', y='Wait Time Mean', hue='Scheduler', data=data)
plt.title('Média de Tempo de Espera para Cada Caso de Teste')
plt.ylabel('Tempo de Espera Médio (ms)')
plt.xlabel('Caso de Teste')
plt.show()

# Gráfico de Throughput para cada caso de teste
plt.figure(figsize=(10, 6))
sns.barplot(x='Test Case', y='Throughput', hue='Scheduler', data=data)
plt.title('Throughput para Cada Caso de Teste')
plt.ylabel('Throughput')
plt.xlabel('Caso de Teste')
plt.show()
