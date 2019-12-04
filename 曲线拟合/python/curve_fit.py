#!/usr/bin/python
# coding=utf-8
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
#vehicle speed is positive


if __name__=="__main__":
    def func(x, a, b):
        return a/abs(x) + b
    Y=[4.055,4.389,4.779999999,5.275,5.8,6.410000001,7.160000001,
    7.985000002,9.075,10.29,11.95,14.155,17.07,14.075,
    11.81,10.18,8.835,7.874999999,7.055,6.384999999,
    5.755000001,5.220000001,4.755000002,4.305,3.89]
    X=[32.66733184,30.64207912,28.54323086,26.23827271,
    24.14554196,22.07827284,19.95740182,18.03579432,
    15.98708071,14.1802682,12.2747261,10.40811087,
    -8.660386425,-10.46596525,-12.41574775,-14.32721676
    ,-16.39831753,-18.27109099,-20.23050452,-22.1563687,
    -24.31255128,-26.47717428,-28.66952311,-31.12983079,-33.75799373]
    x = np.array(X)
    y = np.array(Y)
    f1 =np.polyfit(x, y, 3)
    plt.plot(x,y,'b-',label='data')
    popt, pcov = curve_fit(func, x, y)
    plt.plot(x, func(x, *popt), 'r-', label='fit: a=%5.3f, b=%5.3f' % tuple(popt))
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend()
    # plt.plot(x,np.polyval(f1, x))
    # p1 = np.poly1d(f1)  
    # yvals = p1(x)  
    # plot1 = plt.plot(x, y, 's',label='original values')
    # plot2 = plt.plot(x, yvals, 'r',label='polyfit values')
    # plt.xlabel('x')
    # plt.ylabel('y')
    # plt.legend(loc=4) 
    plt.title('polyfitting')
    plt.show()




















