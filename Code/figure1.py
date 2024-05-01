from functions import *

from functions import *

speed = 10
steps = 6
bound = speed*(steps)

alphabet = 'abcdefghijklmnopqrstuvwxyz'

origin,bounds,probability,boundary,distance,direction = boundary_type1(bound,speed)

fig,axs = pl.subplots(2,3,figsize=(12,8),constrained_layout=True)

for i,ax in enumerate(axs.flatten()):
    ax.set_xlim(speed*bounds[0,0],speed*bounds[0,1])
    ax.set_ylim(speed*bounds[1,0],speed*bounds[1,1])
    ax.xaxis.set_major_locator(MultipleLocator(speed))
    ax.yaxis.set_major_locator(MultipleLocator(speed))
    ax.text(0.01,0.95,'$({})$'.format(alphabet[i]),transform=ax.transAxes)
    ax.set_title('Time = {}s'.format(i))
    ax.grid()
    ax.set_aspect(1)

for iteration in range(1,steps+1):

    ax = axs[(iteration-1)//3,(iteration-1)%3]
    plot_probability(ax,bounds,probability,speed)
    
    probability,weighted_time,probability_weights = advance_timstep(probability,origin,boundary,distance,direction,iteration)
    
    print("Iterations = {}".format(iteration))

pl.savefig('fig1.png',bbox_inches='tight',dpi=300)