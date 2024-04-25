# Importing libraries
import numpy as np
import matplotlib.pyplot as pl
from matplotlib.ticker import MultipleLocator

def boundary_type1(bound=20,speed=10):

    boundary  = []
    distance  = []
    direction = []

    blim = bound//speed
    for i in range(-blim,blim+1):

        boundary  += [np.array([i, blim])]
        distance  += [1]
        direction += ['N']
        
        boundary  += [np.array([i,-blim])]
        distance  += [1]
        direction += ['S']

        boundary  += [np.array([ blim,i])]
        distance  += [1]
        direction += ['E']

        boundary  += [np.array([-blim,i])]
        distance  += [1]
        direction += ['W']

    bounds = [[-blim,blim],[-blim,blim]]
    origin = [blim,blim]

    probability = np.zeros([2*blim+1,2*blim+1])
    probability[blim,blim] = 1

    return(np.array(origin),np.array(bounds),probability,np.array(boundary),np.array(distance),np.array(direction))

def evaluate_polynomial(polynomial,coords):

    value = np.zeros(coords.shape[0])
    for term in polynomial:
        value = value + term[0]*(coords[:,0]**term[1])*(coords[:,1]**term[2])

    return(value)

# Get the coefficients of the x polynomial at given y
def getcoefx_polynomial(polynomial,ycoord=0):
    maxorderx = int(np.max(polynomial[:,1]))
    polycoefx = np.zeros([maxorderx+1])
    for i in range(maxorderx+1):
        selection    = polynomial[:,1]==(maxorderx-i)
        polycoefx[i] = np.sum(polynomial[selection,0]*(ycoord**polynomial[selection,2]))
    return(polycoefx)

# Get the coefficients of the y polynomial at given x
def getcoefy_polynomial(polynomial,xcoord=0):
    maxordery = int(np.max(polynomial[:,2]))
    polycoefy = np.zeros([maxordery+1])
    for i in range(maxordery+1):
        selection    = polynomial[:,2]==(maxordery-i)
        polycoefy[i] = np.sum(polynomial[selection,0]*(xcoord**polynomial[selection,1]))
    return(polycoefy)

def getroots_polynomial(polynomial,speed):

    rescaled_polynomial = np.copy(polynomial)
    rescaled_polynomial[:,0] = rescaled_polynomial[:,0]*speed**(rescaled_polynomial[:,1]+rescaled_polynomial[:,2])

    xroots = np.empty([0,2])
    yroots = np.empty([0,2])

    numroots  = 1
    ycoord    = 0
    while (numroots > 0):
        polycoefx = getcoefx_polynomial(rescaled_polynomial,ycoord) 
        polyrootx = np.roots(polycoefx)
        polyrootx = np.real(polyrootx[np.isreal(polyrootx)])
        numroots  = polyrootx.size
        xroots    = np.append(xroots,np.stack((polyrootx,ycoord*np.ones_like(polyrootx)),axis=1),axis=0)
        ycoord   -= 1

    numroots  = 1
    ycoord    = 0
    while (numroots > 0):
        polycoefx = getcoefx_polynomial(rescaled_polynomial,ycoord) 
        polyrootx = np.roots(polycoefx)
        polyrootx = np.real(polyrootx[np.isreal(polyrootx)])
        numroots  = polyrootx.size
        xroots    = np.append(xroots,np.stack((polyrootx,ycoord*np.ones_like(polyrootx)),axis=1),axis=0)
        ycoord   += 1

    numroots  = 1
    xcoord    = 0
    while (numroots > 0):
        polycoefy = getcoefy_polynomial(rescaled_polynomial,xcoord) 
        polyrooty = np.roots(polycoefy)
        polyrooty = np.real(polyrooty[np.isreal(polyrooty)])
        numroots  = polyrooty.size
        yroots    = np.append(yroots,np.stack((xcoord*np.ones_like(polyrooty),polyrooty),axis=1),axis=0)
        xcoord   -= 1

    numroots  = 1
    xcoord    = 0
    while (numroots > 0):
        polycoefy = getcoefy_polynomial(rescaled_polynomial,xcoord) 
        polyrooty = np.roots(polycoefy)
        polyrooty = np.real(polyrooty[np.isreal(polyrooty)])
        numroots  = polyrooty.size
        yroots    = np.append(yroots,np.stack((xcoord*np.ones_like(polyrooty),polyrooty),axis=1),axis=0)
        xcoord   += 1

    return(xroots,yroots)

def boundary_polynomial(polynomial,speed):

    # Get roots
    xroots,yroots = getroots_polynomial(polynomial,speed)

    # Get bounds
    xmin = int(np.min(xroots[:,0])) - 1
    xmax = int(np.max(xroots[:,0])) + 1
    ymin = int(np.min(yroots[:,1])) - 1
    ymax = int(np.max(yroots[:,1])) + 1

    bounds = np.array([[xmin,xmax],[ymin,ymax]])

    # Creating probability
    probability = np.zeros([ymax-ymin+1,xmax-xmin+1])
    probability[-xmin,-ymin] = 1
    origin = [-xmin,-ymin]

    # Value at origin and rescale polynomial
    valorigin = evaluate_polynomial(polynomial,np.array([[0,0]]))
    rescaled_polynomial = np.copy(polynomial)
    rescaled_polynomial[:,0] = rescaled_polynomial[:,0]*speed**(rescaled_polynomial[:,1]+rescaled_polynomial[:,2])

    # Find boundary points, distance and direction of crossing
    boundary  = np.empty([0,2],dtype=int)
    distance  = np.empty([0],dtype=float)
    direction = np.empty([0],dtype=str)

    x   = np.arange(xmin,xmax+1)
    y   = np.arange(ymin,ymax+1)
    X,Y = np.meshgrid(x,y)
    C   = np.stack((X.flatten(),Y.flatten()),axis=1)
    Z   = np.sign(valorigin)*np.sign(evaluate_polynomial(rescaled_polynomial,C).reshape([ymax-ymin+1,xmax-xmin+1]))
    
    Z1  = np.pad((Z[:-1,:]-Z[1:,:]),((1,0),(0,0)),'constant',constant_values=0) # North
    Z2  = np.logical_or((Z1==2),np.logical_and((Z==0),(Z1>0)))
    
    positions = C[Z2.flatten()]
    boundary  = np.append(boundary,positions,axis=0)
    for pos in positions:
        sta = pos - np.array([0,1])
        idx = yroots[:,0]==pos[0]
        dst = np.array([np.min(np.abs(yroots[idx,1] - sta[1]))])
        distance  = np.append(distance,dst,axis=0)
    direction = np.append(direction,np.full([positions.shape[0]],'N'),axis=0)

    print(boundary.shape,distance.shape,direction.shape)

    Z1  = np.pad((Z[1:,:]-Z[:-1,:]),((0,1),(0,0)),'constant',constant_values=0) # South
    Z2  = np.logical_or((Z1==2),np.logical_and((Z==0),(Z1>0)))

    positions = C[Z2.flatten()]
    boundary  = np.append(boundary,C[Z2.flatten()],axis=0)
    for pos in positions:
        sta = pos + np.array([0,1])
        idx = yroots[:,0]==pos[0]
        dst = np.array([np.min(np.abs(yroots[idx,1] - sta[1]))])
        distance  = np.append(distance,dst,axis=0)
    direction = np.append(direction,np.full([positions.shape[0]],'S'),axis=0)
    
    print(boundary.shape,distance.shape,direction.shape)

    Z1  = np.pad((Z[:,:-1]-Z[:,1:]),((0,0),(1,0)),'constant',constant_values=0) # East
    Z2  = np.logical_or((Z1==2),np.logical_and((Z==0),(Z1>0)))

    positions = C[Z2.flatten()]
    boundary  = np.append(boundary,C[Z2.flatten()],axis=0)
    for pos in positions:
        sta = pos - np.array([1,0])
        idx = xroots[:,1]==pos[1]
        dst = np.array([np.min(np.abs(xroots[idx,0] - sta[0]))])
        distance  = np.append(distance,dst,axis=0)
    direction = np.append(direction,np.full([positions.shape[0]],'E'),axis=0)
    
    Z1  = np.pad((Z[:,1:]-Z[:,:-1]),((0,0),(0,1)),'constant',constant_values=0) # West
    Z2  = np.logical_or((Z1==2),np.logical_and((Z==0),(Z1>0)))

    positions = C[Z2.flatten()]
    boundary  = np.append(boundary,positions,axis=0)
    for pos in positions:
        sta = pos + np.array([1,0])
        idx = xroots[:,1]==pos[1]
        dst = np.array([np.min(np.abs(xroots[idx,0] - sta[0]))])
        distance  = np.append(distance,dst,axis=0)
    direction = np.append(direction,np.full([positions.shape[0]],'W'),axis=0)

    return(origin,bounds,probability,boundary,distance,direction)

def advance_timstep(current_probability,origin,boundary,distance,direction,timestep):

    weighted_time           = 0
    probability_weights     = 0
    future_probability      = np.zeros_like(current_probability)
    direction_list          = ['N','S','E','W']
    
    for d in direction_list:

        interm_probability                      = np.zeros_like(current_probability)
        if (d=='N'): interm_probability[ 1:,:]  = 0.25*current_probability[:-1,:]
        if (d=='S'): interm_probability[:-1,:]  = 0.25*current_probability[ 1:,:]
        if (d=='E'): interm_probability[:, 1:]  = 0.25*current_probability[:,:-1]
        if (d=='W'): interm_probability[:,:-1]  = 0.25*current_probability[:, 1:]
        
        interm_boundary                         = boundary[direction==d] + origin
        interm_position                         = (interm_boundary[:,1]*interm_probability.shape[1]) + interm_boundary[:,0]
        interm_distance                         = distance[direction==d]
        interm_crossing                         = np.take(interm_probability,interm_position)
        weighted_time                           = weighted_time + (timestep*np.sum(interm_distance*interm_crossing))
        probability_weights                     = probability_weights + np.sum(interm_crossing)
        future_probability                      = future_probability + interm_probability

    relative_boundary = boundary + origin
    indices = np.unique((relative_boundary[:,1]*future_probability.shape[1]) + relative_boundary[:,0])
    np.put(future_probability,indices,0)

    return(future_probability,weighted_time,probability_weights)

def plot_polynomial(axs,polynomial,bounds,speed,resolution=10):

    xmin = bounds[0,0]
    xmax = bounds[0,1]
    ymin = bounds[1,0]
    ymax = bounds[1,1]

    x = speed*np.linspace(xmin,xmax,((xmax-xmin)*resolution)+1)
    y = speed*np.linspace(ymin,ymax,((ymax-ymin)*resolution)+1)
    X,Y = np.meshgrid(x,y)
    coords = np.vstack([X.flatten(),Y.flatten()]).transpose()
    z = evaluate_polynomial(polynomial,coords).reshape(X.shape)
    img = axs.contour(x,y,z,levels=[0],colors=['k'])
    axs.set_xlim(speed*(xmin-1),speed*(xmax+1))
    axs.set_ylim(speed*(ymin-1),speed*(ymax+1))
    return(img)

def plot_outline(axs,boundary,speed):

    temp_boundary = np.unique(boundary[:,0] + (boundary[:,1]*1j))
    temp_boundary = temp_boundary[np.argsort(np.angle(temp_boundary))]
    temp_boundary = np.pad(temp_boundary,(0,1),'wrap')
    axs.plot(speed*np.real(temp_boundary),speed*np.imag(temp_boundary),'k-')
    return()

def plot_boundary(axs,boundary,distance,direction,speed):

    for point,path,dirn in zip(boundary,distance,direction):
        axs.plot(speed*point[0],speed*point[1],'kx')
        if (dirn=='N'):
            axs.plot(speed*point[0],speed*(point[1]-1),'k.')
            axs.plot(speed*point[0],speed*(point[1]-1+path),'r^')
            axs.plot([speed*point[0],speed*point[0]],[speed*(point[1]-1),speed*point[1]],'r-')
        elif (dirn=='S'):
            axs.plot(speed*point[0],speed*(point[1]+1),'k.')
            axs.plot(speed*point[0],speed*(point[1]+1-path),'gv')
            axs.plot([speed*point[0],speed*point[0]],[speed*(point[1]+1),speed*(point[1])],'g-')
        elif (dirn=='E'):
            axs.plot(speed*(point[0]-1),speed*point[1],'k.')
            axs.plot(speed*(point[0]-1+path),speed*point[1],'b>')
            axs.plot([speed*(point[0]-1),speed*(point[0])],[speed*point[1],speed*point[1]],'b-')
        elif (dirn=='W'):
            axs.plot(speed*(point[0]+1),speed*point[1],'k.')
            axs.plot(speed*(point[0]+1-path),speed*point[1],'m<')
            axs.plot([speed*(point[0]+1),speed*(point[0])],[speed*point[1],speed*point[1]],'m-')
    
    axs.plot([],[],'r^',label='Crossing North')
    axs.plot([],[],'gv',label='Crossing South')
    axs.plot([],[],'b>',label='Crossing East')
    axs.plot([],[],'m<',label='Crossing West')

    return()