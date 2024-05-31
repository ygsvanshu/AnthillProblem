from functions import *

speed = 10
steps = 1000
bound = speed*(steps)

alphabet = 'abcdefghijklmnopqrstuvwxyz'

origin,bounds,probability,boundary,distance,direction = boundary_type1(bound,speed)

fig,axs = pl.subplots(1,3,figsize=(10,4),constrained_layout=True)

axs[0].set_xscale('log')
axs[0].set_yscale('log')
axs[0].set_xlabel('time [s]')
axs[0].set_ylabel('probability of ants finding food\nat given time')
axs[0].text(0.02,0.95,'$(a)$',transform=axs[0].transAxes,bbox=dict(facecolor='white', edgecolor='none', boxstyle='round'))

axs[1].set_xscale('log')
# axs[1].set_yscale('log')
axs[1].set_xlabel('time [s]')
axs[1].set_ylim(0,1)
axs[1].set_ylabel('cumulative probability of ants\nwhich have found food')
axs[1].text(0.02,0.95,'$(b)$',transform=axs[1].transAxes,bbox=dict(facecolor='white', edgecolor='none', boxstyle='round'))

axs[2].set_xscale('log')
axs[2].set_yscale('log')
axs[2].set_xlabel('time [s]')
axs[2].set_ylabel('mean time for ants to find food')
axs[2].text(0.02,0.95,'$(c)$',transform=axs[2].transAxes,bbox=dict(facecolor='white', edgecolor='none', boxstyle='round'))

ttot = 0
ptot = 0
for iteration in range(1,steps+1):

    psum = 0
    pold = 1
    for i in range(2*steps):
        p,q = probability[i,-i].as_integer_ratio()
        psum += probability[i,-i]
        probability[i,-i:] = 0

    ttot += psum*iteration
    ptot += psum

    if (psum!=0): 
        prat = (psum/pold)
        pold = psum
        
        axs[0].plot(iteration,psum,'k.')
        axs[1].plot(iteration,ptot,'k.')
        axs[2].plot(iteration,ttot,'k.')

    probability,weighted_time,probability_weights = advance_timstep(probability,origin,boundary,distance,direction,iteration)
    
pl.savefig('question2.png',bbox_inches='tight',dpi=300)