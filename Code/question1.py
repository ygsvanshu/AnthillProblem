from functions import *

speed = 10
bound = 20
steps = 100
toler = 1e-6
stout = 10

origin,bounds,probability,boundary,distance,direction = boundary_type1(bound,speed)

fig,axs = pl.subplots(1,2,figsize=(10,5),constrained_layout=True)
ax1 = axs[0]
ax2 = axs[1]

plot_boundary(ax1,boundary,distance,direction,speed)
plot_outline(ax1,boundary,speed)
ax1.xaxis.set_major_locator(MultipleLocator(speed))
ax1.yaxis.set_major_locator(MultipleLocator(speed))
ax1.grid()
ax1.set_aspect(1)
ax1.text(0.05,0.9,'$(a)$',transform=ax1.transAxes)
ax1.legend(ncol=2,loc='upper center',bbox_to_anchor=(0.5,1.2))

time_array = []
weight_array = []
total_time = 0
total_weight = 0
iteration = 1

while ((abs(1-total_weight)>=toler) and (iteration<=steps)):
    
    probability,weighted_time,probability_weights = advance_timstep(probability,origin,boundary,distance,direction,iteration)
    
    time_array += [weighted_time]
    weight_array += [probability_weights]

    total_time += weighted_time
    total_weight += probability_weights

    iteration += 1

    if (iteration%stout==0):
        
        ax2.cla()
        ax2.plot(np.cumsum(np.array(time_array)),'k.-',label='Average time taken')
        ax2.plot(time_array,'r.-',label='Probability weighted time per timestep')
        ax2.plot(np.cumsum(np.array(weight_array)),'g.-',label='Cumulative probability')
        ax2.plot(weight_array,'b.-',label='Probability weights per timestep')
        ax2.axhline(total_time,color='gray',linestyle='--')
        ax2.axhline(total_weight,color='limegreen',linestyle='--')
        ax2.axhline(0,color='magenta',linestyle='--')
        ax2.set_xlabel('Timesteps')
        ax2.text(0.02,0.9,'$(b)$',transform=ax2.transAxes)
        ax2.legend(loc='upper center',bbox_to_anchor=(0.5,1.3))
        pl.savefig('question1.png',bbox_inches='tight',dpi=300)

        print()
        print("Average time for ants to reach food = {:.3g} seconds".format(total_time))
        print("Cumulative probability = {:.3g}".format(total_weight))
        print("Iterations = {}".format(iteration))
        print()