# -*- coding: utf-8 -*-
"""
Created on Fri Sep  4 14:43:23 2020

@author: Adrian Gallo
"""
import numpy as np
import random

class SimpleGenetic():
    def setMaxGen(self,a):
        self.MaxGen = a
        
    def setNind(self,a):
        self.NInd = a
        
    def setLInd(self,a):
        self.LInd=a
        
    def setPc(self,a):
        self.Pc = a
        
    def setPm(self,a):
        self.Pm = a
        
    def setBestInitial(self,a):
        self.BestInitial = a
        
    def setBestLast(self,a):
        self.BestLast = a
        
    def createPop(self):
        return np.random.randint(0,2, (self.NInd,self.LInd+2))
    
    def setFit(self,a):#asigna el valor del fitness de cada individuo
        for i in range(self.NInd):
            a[i][self.LInd]=(a[i,0:self.LInd].sum())
    
    def setFitAcm(self,a):#asigna el valor del fitness acumulado de cada individuo
        cont = 0
        for i in range(self.NInd):
            cont = cont + a[i,self.LInd] 
            a[i, self.LInd + 1] = cont
    
    def selectParent(self,a):#Selecciona el padre
            
        rand = random.random()
        
        rulet = rand*a[self.NInd-1,self.LInd+1] #Numero aleatoreo multiplicado por el fitness total de la poblacion
        #print(rulet)
        
        for i in range(self.NInd-1):
            if(a[i,self.LInd+1]<rulet and rulet<a[i+1,self.LInd+1]):
                return a[i]
        return a[self.NInd-1]
        #parent2 = random.choice(a)
        #crossing(parent1,parent2)
        #print(random.choice(a))
        
    def crossing(self,parent1, parent2):
        if(random.random() > self.Pc):   #Evalua la probabilidad de cruce 
                #Si No existe cruza los hijos son iguales a los padres
            a = parent1 
            b = parent2
        else:
            rand=random.randrange(self.LInd) #Index de cruza 
            a = np.append(parent1[0:rand],parent2[rand:self.LInd+2])
            b = np.append(parent2[0:rand],parent1[rand:self.LInd+2]) 
        
        return self.mutation(a,b)#Llama a mutation 
    
    def mutation(self, a,b):
        for i in range(self.LInd):
            rand = random.random()
            if(self.Pm>rand):
                if(a[i]==1):
                    a[i]=0
                else:
                    a[i]=1
                    
        for i in range(self.LInd):
            rand = random.random()
            if(self.Pm>rand):
                if(b[i]==1):
                    b[i]=0
                else:
                    b[i]=1
                
        return (a,b)
    
    def main(self):
    
        np.set_printoptions(suppress=True, threshold= np.inf, linewidth=135)#Quita la notacion cientifica de numpy
    
        #self.NInd = 50 #Numero de individuos por poblacion 
        #self.LInd = 20 #Largo de individuos
        #self.Pc = 0.7 #Probabilidad de cruza
        #self.Pm = 0.0555 #Probabiidad mutacion 
        #self.MaxGen = 100 #Maximo numero de generaciones
        
        self.InitialPop = np.zeros((self.NInd,self.LInd+2),dtype=int) #Crea la base para la primera población( se ponen dos columnas mas, una para el fitness y otra para el acumulado)
        self.InitialPop = self.createPop() #genera 1's y 0's en InitialPop
        #print(InitialPop)
        #print(type(InitialPop))
        
        
        self.CurrentPop = self.InitialPop #La primera poblacion se convierte en la actual
        self.NextPop = np.zeros((self.NInd,self.LInd+2),dtype=int)#Siguiente poblacion a cero
        
        self.generation = 1 #Generacion actual 
    #****************************************************************************************************************    
        while(self.generation <= self.MaxGen or np.amax(self.CurrentPop[:,self.LInd:self.LInd+1]) == self.LInd): #Mientras no se llegue a la poblacion maxima o se llegue al fitness deseado 
            
            # print('Generacion numero: ', generation)
            # print(np.amax(CurrentPop[:,20:21]))
            
            self.NextPop = np.zeros((self.NInd,self.LInd+2),dtype=int)#siguiente poblacion a cero
            
            self.setFit(self.CurrentPop) #obtiene el fitnes de la poblacion inicial
            self.setFitAcm(self.CurrentPop) #Obtiene el fitness acumulado de la poblacion inicial
            #print(CurrentPop)
            
            chro=0 #index del cromosoma actual
            
    #=============================================================================
            while(chro<=self.NInd-1): #Mientras se llena la poblacion actual
                
                #print(chro)
                parent1 = self.selectParent(self.CurrentPop) #Selecciona padre1
                parent2 = self.selectParent(self.CurrentPop) #Selecciona padre2
                childs = self.crossing(parent1, parent2) #Selecciona hijos
                child1= childs[0]    #Asigna Hijo1
                child2= childs[1]    #Asigna Hijo2
                
                self.NextPop[chro] = child1  #Append child to nextpop
                self.NextPop[chro+1] = child2 #Append child to nextpop
                
                
                #print(NextPop)
                chro+=2 #Aumenta el cromosoma en 2
    #=============================================================================
            
            self.NextPop[:,self.LInd:]=0 #Las columnas de fitness y fitness acumulado de nextpop a cero
            
            self.setFit(self.NextPop) #obtiene el fitnes 
            self.setFitAcm(self.NextPop) #Obtiene el fitness acumulado4
        
            #print(NextPop)
        
            self.CurrentPop = self.NextPop #La poblacion siguiente se convierte en la actual
            self.generation+=1
    #******************************************************************************************************************       
        
            self.setBestInitial(np.amax(self.InitialPop[:,self.LInd:self.LInd+1]))
            self.setBestLast(np.amax(self.CurrentPop[:,self.LInd:self.LInd+1]))
        
    def printResults(self):
        print()
        print('La ultima Poblacion es:')
        print()
        print(self.CurrentPop)
        print()
        print()
        print('La mejor calificación de la generacion numero 1 es: ', self.BestInitial)
        print('La mejor calificación de la generacion numero ',self.generation-1,' es: ', self.BestLast)#Imprime la calificacion del mejor cromosoma de la poblacion 
        

if __name__ == '__main__':

    SG = SimpleGenetic()#Declaracion de la clase principal
    
    SG.setNind(50)#Numero de individuos por poblacion
    SG.setLInd(20)#Largo de individuos
    SG.setPc(0.70)#Probabilidad de cruza
    SG.setPm(0.0555)#Probabiidad mutacion
    SG.setMaxGen(100)
    
    SG.main()
    SG.printResults()