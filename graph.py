import matplotlib.pyplot as plt
import csv
import glob

def createGraph(title,name):
	print name
	dataX = []
	dataY = []
	labelX = ""
	labelY = "" 
	with open(name, 'rb') as csvfile:
		spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
		top = 0
		for row in spamreader:
			if top != 0:			
				dataX.append(float(row[0]))
				dataY.append(float(row[1]))
			else:
				top = top + 1 
				labelX = row[0]
				labelY = row[1]
			# print(row)
			# print ', '.join(row)

	plt.title(title)
	#lone = plt.plot(dataX,dataY,'bs',linestyle='--')
	lone = plt.plot(dataX,dataY)

	# ltwo, = plt.plot(data['times'],data['twenty'],marker='o',linestyle='--',color='red')
	# lthree, = plt.plot(data['times'],data['thirty'],'g^',linestyle='--')
	# lfour, = plt.plot(data['times'],data['fourty'],'g^',linestyle=':',color='orange')
	#,"MR 0.20","MR 0.30","MR 0.40"
	plt.ylabel(labelY)
	plt.xlabel(labelX)
	plt.legend([lone],["MR 0.10"])
	plt.show()


data = [
 	"danny"	
]

titles = [
	"Test"	
]

# for a simulation of 500 sim rounds with a GR of 0.1, we see a pretty linear relation without / with firefighters 

for idx, graph in enumerate(data):
	createGraph(titles[idx],graph+".csv")
	#createGraph(graph)#graph+".csv")
