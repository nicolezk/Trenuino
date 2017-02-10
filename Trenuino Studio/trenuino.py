#Bibliotecas importadas
import numpy
import math as math
import matplotlib.pyplot as plt

#Funcao retirada de:
#http://stackoverflow.com/questions/24467972/calculate-area-of-polygon-given-x-y-coordinates
def PolygonArea(corners):
    n = len(corners) # of corners
    area = 0.0
    for i in range(n):
        j = (i + 1) % n
        area += corners[i][0] * corners[j][1]
        area -= corners[j][0] * corners[i][1]
    area = abs(area) / 2.0
    return area

#Funcao para gerar uma imagem da regiao maepada
def processaMedicao(filename):
    
    ### I N I C I A L I Z A C A O   D E   M E D I D A S ###
    
    #Leitura das medicoes em coordenadas polares (grau, distancia)       
    medidas = numpy.loadtxt(open(filename, "rb"), delimiter=",")
    
    #Converte coordenada polar para coordenada "retangular" (x,y)
    coordenadas = []
    for i in medidas:
        coordenadas.append((math.sin(math.radians(i[0])) * i[1], math.cos(math.radians(i[0])) * i[1]));

    #Como o centro/eixo esta em (0,0), algumas coordendas tem x ou y negativo
    #assim, deslocamos todas as coordendas para valores x e y positivas
    menorX = 0;
    menorY = 0;
    for i in coordenadas:
        if i[0] < menorX:
            menorX = i[0]
            if i[1] < menorY:
                menorY = i[1]   
    coordenadasPositivas = []
    for i in coordenadas:
        coordenadasPositivas.append(((i[0] + abs(menorX)), (i[1] + abs(menorY))))
    
    
    ### T R A T A M E N T O   D E   R U I D O ###
    
    #Cria um vetor de coordendas, identificando aquelas com ruido (colca-se (0,0))
    #O ruido eh dado por coordendas que nao estao numa faixa de 95% e 105% da media
    #de seus vizinhos
    coordenadasNoOutlier = []
    for i in range (1, len(coordenadasPositivas) - 1):
        if ((coordenadasPositivas[i][0] < 1.05 * ((coordenadasPositivas[i-1][0] + coordenadasPositivas[i+1][0])/2)) and \
             (coordenadasPositivas[i][0] > 0.95 * ((coordenadasPositivas[i-1][0] + coordenadasPositivas[i+1][0])/2))) and \
             ((coordenadasPositivas[i][1] < 1.05 * ((coordenadasPositivas[i-1][1] + coordenadasPositivas[i+1][1])/2)) and \
             (coordenadasPositivas[i][1] > 0.95 * ((coordenadasPositivas[i-1][1] + coordenadasPositivas[i+1][1])/2))):
                 coordenadasNoOutlier.append(coordenadasPositivas[i])
        #Aqui inserimos as coordenadas ruidosas como coordenada (0,0)
        else:
            coordenadasNoOutlier.append((0, 0))
     
    #Para as coordenadas ruidosas (identificadas por (0,0)), geramos um ponto
    #intermediario entre o seu valor anterior e a proxima coordenada nao ruidosa
    coordenadasFinais = []
    for i in range (1, len(coordenadasNoOutlier) - 1):   
        if coordenadasNoOutlier[i] != (0, 0):
            coordenadasFinais.append(coordenadasNoOutlier[i])
        else:
            j = i;  #Indice da primeira coordenada a esquerda nao outlier
            while coordenadasNoOutlier[j] == (0, 0):
                j = j + 1
            x_aux = (coordenadasNoOutlier[j][0] + coordenadasFinais[i-2][0])/2
            y_aux = (coordenadasNoOutlier[j][1] + coordenadasFinais[i-2][1])/2
            coordenadasFinais.append((x_aux,y_aux))
            

    ### I M P R E S S A O   D A   I M A G E M ###                                                      
    x_points = []
    y_points = []

    for i in coordenadasFinais:
        x_points.append(i[0]);
        y_points.append(i[1]);
                                              
    fig = plt.figure()
    ax = fig.add_subplot(111)
    p = ax.plot(x_points, y_points, 'b')
    ax.set_xlabel('X (cm)')
    ax.set_ylabel('Y (cm)')
    ax.set_title('Planta nas coordenadas X e Y')
    fig.show()
    plt.savefig("resultados/IMAGEM.png")
    
    #Inicio da secao de impressao de dados
    print('\n\n\n')
    
    ### C A L C U L O   D O   P E R I M E T R O ###
    perimetro = 0;
    for i in range (len(coordenadasFinais) - 1, 0, -1):
        perimetro = perimetro + math.sqrt((coordenadasFinais[i][0] - coordenadasFinais[i-1][0])**2 + (coordenadasFinais[i][1] - coordenadasFinais[i-1][1])**2)
    print(('Perimetro: %.2f' % perimetro) + 'cm')
    
    ### C A L C U L O   D A   A R E A ###
    area = PolygonArea(coordenadasFinais)
    print(('Area: %.2f' % area) + 'cm^2')
    
    #Fim da secao de impressao de dados
    print('\n\n\n')
    
    ### S A L V A   I N F O S ###
    target = open('resultados/DADOS DA MEDICAO.txt', 'w')
    target.truncate()
    target.write('Dados obtidos da medicao do Trenuino: \n\n')
    target.write('Perimetro: %.2f' % perimetro)
    target.write('cm \n')
    target.write('Area: %.2f' % area)
    target.write('cm^2 \n')
    target.close
    
    
processaMedicao('MEDIDAS.csv')
    



