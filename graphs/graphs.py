# Importando as bibliotecas necessárias
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Carregando os dados do arquivo CSV
file_path = './results.csv'
data = pd.read_csv(file_path)

# Definindo o estilo dos gráficos
sns.set(style="whitegrid")

# Função para criar histograma
def create_histogram(data, column, title):
    plt.figure(figsize=(8, 6))
    sns.histplot(data[data['Scheduler'] == 'FIFO'][column], kde=True, color="skyblue", label='FIFO')
    sns.histplot(data[data['Scheduler'] == 'Nativo'][column], kde=True, color="red", label='Nativo')
    plt.title(title)
    plt.legend()
    plt.show()

# Função para criar boxplot
def create_boxplot(data, column, title):
    plt.figure(figsize=(8, 6))
    sns.boxplot(x='Scheduler', y=column, data=data)
    plt.title(title)
    plt.show()

# Criando e mostrando os gráficos individualmente
# Histogramas
create_histogram(data, 'Wait Time Mean', 'Histograma do Tempo de Espera Médio')
create_histogram(data, 'Execution Time Mean', 'Histograma do Tempo de Execução Médio')
create_histogram(data, 'Throughput', 'Histograma de Throughput')

# Boxplots
create_boxplot(data, 'Wait Time Mean', 'Boxplot do Tempo de Espera Médio')
create_boxplot(data, 'Execution Time Mean', 'Boxplot do Tempo de Execução Médio')
create_boxplot(data, 'Throughput', 'Boxplot de Throughput')
