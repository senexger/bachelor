import numpy as np
from matplotlib import pyplot as plt

def main():
    array = np.genfromtxt("/home/walther/Documents/bachelor/Data/broad_successratio/successratio_broadcast_F100_04-14-00:18:29.csv", delimiter=",", dtype="int")
    array.shape = (2, 3, 160)

    counter1 = 0
    counter2 = 0
    counter3 = 0

    for i in range(0,160):
        if (array[0,0,i]):
            counter1 += 1
        if (array[0,0,i] and array[0,1,i]):
            counter2 += 1
        if (array[0,0,i] and array[0,1,i]) and array[0,2,i]:
            counter3 += 1

    print(counter1)
    print(counter2)
    print(counter3)
    print("success ratio 1: " + str(successPercentage(counter1)))
    print("success ratio 2: " + str(successPercentage(counter2)))
    print("success ratio 3: " + str(successPercentage(counter3)))

    # print(array[0,0,:])
    # print(array[0,1,:])
    # print(array)

    # plt.title("Success ratio for each node") 
    # plt.xlabel("Node number") 
    # plt.ylabel("Success ratio in %") 

    x = np.linspace(1,2,100)

    # fig, ax = plt.subplots()  # Create a figure and an axes.
    # ax.plot(x, x, label='linear')  # Plot some data on the axes.
    # ax.plot(x, x**2, label='quadratic')  # Plot more data on the axes...
    # ax.plot(x, x**3, label='cubic')  # ... and some more.
    # ax.set_xlabel('x label')  # Add an x-label to the axes.
    # ax.set_ylabel('y label')  # Add a y-label to the axes.
    # ax.set_title("Simple Plot")  # Add a title to the axes.
    # ax.legend()  # Add a legend.
    
    plt.plot(x, x, label='linear')  # Plot some data on the (implicit) axes.
    plt.plot(x, x**2, label='quadratic')  # etc.
    plt.plot(x, x**3, label='cubic')
    plt.xlabel('x label')
    plt.ylabel('y label')
    plt.title("Simple Plot")
    plt.legend()

    
    # plt.plot(array[:1],"ob") 
    # plt.show()

def successPercentage(counter):
    return 100-(160/counter)

if __name__ == "__main__":
    main()