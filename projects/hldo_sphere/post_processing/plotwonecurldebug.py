#!/usr/bin/env python3

import click
import pandas as pd 
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.ticker import FormatStrFormatter

@click.command()
@click.option('-f', '--file', required=True, help='csv file containing the results to plot')
@click.option('-l', '--log', required=False, is_flag=True, help='Indicates if log log plot should be drawn')

def plot(file, log):

    plt.rcParams.update({
        "text.usetex": True,
        "font.family" : 'Palatino', 
        "font.size" : 11,
    })

    df = pd.read_csv(file)

    # exclude supremum norm and meta data
    df_y = df.iloc[:,4:]

    # get h values
    df_h = df[["hMax"]].iloc[0:]
    x = df_h.to_numpy().flatten()
    df_x_label = "h"

    # print table that is, print all l2 errors
    print(df_y)

    fig, axs = plt.subplots(1,1)

    # rounds the axis on two siginficant positions
    axs.yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
    axs.xaxis.set_major_formatter(FormatStrFormatter('%.2f'))

    # add the three lines
    yl2 = df_y.iloc[0:,0].to_numpy()
    yref = x * x

    # compute rates
    n = yl2.size;
    ratel2 = np.log(yl2[1:n] / yl2[0:n-1]) / np.log(x[1:n] / x[0:n-1])
    
    print("Rate for l2 erro norm", ratel2)

    if(log):
        axs.loglog(x,yref)
        axs.loglog(x,yl2)
    else:
        axs.plot(x,yref)
        axs.plot(x,yl2)


    # set labels and swap the x direction to get decreasing error
    axs.set(ylabel="$L^2$ error", xlabel=df_x_label, xlim=(max(x), min(x)))
    axs.legend([ "$h^2$","l2 error"])

    #plt.title("$L^2$-Error of Hodge Laplacian Source Problems")
    plt.savefig("whitneyonecurl.png", dpi=300)
    plt.show()



if __name__ == '__main__':
    plot()
