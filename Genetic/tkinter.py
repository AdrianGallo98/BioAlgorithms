# -*- coding: utf-8 -*-
"""
Created on Thu Sep 24 00:36:13 2020

@author: Adrian Gallo
"""

from tkinter import ttk
import tkinter as tk
from Genetic_Algorithm import SimpleGenetic

class Genetic():
    
    choicesGenerations = [50,100,150,200,250,300,400,500,800,1000]
    choicesChro = [20,50,100,150,200,300]
    choicesGenes = [15,18,20,25,30]
    
    
    def __init__(self,window):
        self.wind = window
        self.wind.geometry('920x700')
        self.wind.title('Genetic Algorith')
        
        self.results = tk.Text(self.wind)
        self.results.pack(fill='both')
        
        self.frame1 = tk.Frame(self.wind)
        self.frame1.pack();
        self.frame2 = tk.Frame(self.wind)
        self.frame2.pack();
        self.frame3 = tk.Frame(self.wind)
        self.frame3.pack();
        self.frame4 = tk.Frame(self.wind)
        self.frame4.pack();
        self.frame5 = tk.Frame(self.wind)
        self.frame5.pack();
        
        self.labelGenerations = tk.Label(self.frame1, text='Generations: ')
        self.labelGenerations.pack(side='left')
        self.cbGenerations = ttk.Combobox(self.frame1, values=self.choicesGenerations,state="readonly")
        self.cbGenerations.set('Generations')
        self.cbGenerations.pack(side='right')
        
        self.labelChro = tk.Label(self.frame2, text='Chromosomes: ')
        self.labelChro.pack(side='left')
        self.cbChro = ttk.Combobox(self.frame2, values=self.choicesChro,state="readonly")
        self.cbChro.set('Chromosomes')
        self.cbChro.pack(side='right')
        #self.cbChro.grid()
        
        self.labelGenes = tk.Label(self.frame3, text='Genes: ')
        self.labelGenes.pack(side='left')
        self.cbGenes = ttk.Combobox(self.frame3, values=self.choicesGenes,state="readonly")
        self.cbGenes.set('Genes')
        self.cbGenes.pack(side='right')
        
        self.labelGenerations = tk.Label(self.frame4, text='Crossing: ')
        self.labelGenerations.pack(side='left')
        self.PCross = ttk.Entry(self.frame4)
        self.PCross.insert(0,'0.70')
        self.PCross.pack(side='right')
        
        self.labelGenerations = tk.Label(self.frame5, text='Mutation: ')
        self.labelGenerations.pack(side='left')
        self.PMutation = ttk.Entry(self.frame5)
        self.PMutation.insert(0, '0.0555')
        self.PMutation.pack(side='right')
        
        self.message = tk.Label(text = '', fg = 'red')
        self.message.pack()
        
        self.buttonSubmit = ttk.Button(self.wind, text='Submit', command = self.Test)
        self.buttonSubmit.pack()
        
        self.linebreak = tk.Label(text='')
        self.linebreak.pack()
        self.linebreak.pack()
        
        self.Result1 = tk.Label(text='')
        self.Result1.pack()
        
        self.Result2 = tk.Label(text='')
        self.Result2.pack()
        
        # frame2 = tk.PhotoImage(file="D:/UAA/7 semestre/Metaheuristicas 1/Genetic Algorithm/SimpleGenetic/procesando.gif", format="gif -index 2")
        
        # bg_Image = tk.PhotoImage(file = "D:/UAA/7 semestre/Metaheuristicas 1/Genetic Algorithm/SimpleGenetic/procesando.gif")
        # canvas = tk.Canvas(master, width = 500, height = 500, bg = '#d3d3d3')
        # canvas.pack()
        # canvas.create_image(50, 10, image=bg_Image, anchor='nw')
        
    def Test(self):
        #print("hola")
        self.results.configure(state='normal')
        self.results.delete('1.0','end')
        try:
            self.message['text']=''
            
            SG.setMaxGen(int(self.cbGenerations.get()))
            SG.setNind(int(self.cbChro.get()))
            SG.setLInd(int(self.cbGenes.get()))
            SG.setPc(float(self.PCross.get()))
            SG.setPm(float(self.PMutation.get()))
        
            SG.main()
            
            self.results.configure(state='normal')
            self.results.insert(1.0,SG.CurrentPop)
            self.results.configure(state='disabled')
            
            self.Result1['text'] = 'La mejor calificacion de la generacion 1 es: {}'.format(SG.BestInitial)
            self.Result2['text'] = 'La mejor calificacion de la generacion {} es: {}'.format(SG.generation-1,SG.BestLast)
            
            #print('Terminado')
        except ValueError:
            self.message['text']='Selecciona los campos requeridos'
        
        
        
        
if __name__ == '__main__':
    SG = SimpleGenetic()
    window = tk.Tk()
    application = Genetic(window)
    window.mainloop()