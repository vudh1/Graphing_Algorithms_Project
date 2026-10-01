from PIL import Image, ImageDraw, ImageFont
import math
import networkx as nx

W,H=900,520
BG=(13,17,23); PANEL=(22,27,34); TEXT=(230,237,243); MUTED=(139,148,158)
BLUE=(88,166,255); PURPLE=(188,140,255); GREEN=(46,160,67); ACCENT=(255,200,80)

def font(size,bold=False,mono=False):
    base="/usr/share/fonts/truetype/dejavu/"
    if mono:
        name="DejaVuSansMono-Bold.ttf" if bold else "DejaVuSansMono.ttf"
    else:
        name="DejaVuSans-Bold.ttf" if bold else "DejaVuSans.ttf"
    return ImageFont.truetype(base+name,size)

n=36
p=2*math.log(n)/n
er=nx.gnp_random_graph(n,p,seed=12)
ba=nx.barabasi_albert_graph(n,5,seed=14)

def metrics(g):
    component=g if nx.is_connected(g) else g.subgraph(max(nx.connected_components(g),key=len))
    return g.number_of_edges(),nx.diameter(component),nx.transitivity(g)

def mapped(g,box):
    pos=nx.spring_layout(g,seed=2,iterations=80)
    xs=[v[0] for v in pos.values()]; ys=[v[1] for v in pos.values()]
    x0,y0,x1,y1=box
    return {k:(x0+(x-min(xs))/(max(xs)-min(xs))*(x1-x0),
               y0+(y-min(ys))/(max(ys)-min(ys))*(y1-y0)) for k,(x,y) in pos.items()}

erp=mapped(er,(50,150,405,410)); bap=mapped(ba,(495,150,850,410))
er_edges=list(er.edges()); ba_edges=list(ba.edges())
er_m=metrics(er); ba_m=metrics(ba)
frames=[]

for i in range(60):
    im=Image.new("RGB",(W,H),BG); d=ImageDraw.Draw(im)
    d.text((34,22),"Graphing Algorithms Project",font=font(28,True),fill=TEXT)
    d.text((34,58),"Erdős–Rényi vs Barabási–Albert network experiments",font=font(16),fill=MUTED)
    d.rounded_rectangle((28,96,432,478),radius=18,fill=PANEL)
    d.rounded_rectangle((468,96,872,478),radius=18,fill=PANEL)
    d.text((52,112),"Erdős–Rényi",font=font(20,True),fill=BLUE)
    d.text((492,112),"Barabási–Albert",font=font(20,True),fill=PURPLE)
    ratio=0 if i<5 else (1 if i>=40 else (i-5)/35)

    def draw_graph(g,pos,edges,color):
        for u,v in edges[:int(len(edges)*ratio)]:
            d.line((*pos[u],*pos[v]),fill=(63,70,80),width=1)
        for node in list(g.nodes())[:int(g.number_of_nodes()*min(1,ratio*1.25))]:
            x,y=pos[node]; r=3.2+min(5,g.degree[node]/4)
            d.ellipse((x-r,y-r,x+r,y+r),fill=color,outline=(210,220,230))

    draw_graph(er,erp,er_edges,BLUE); draw_graph(ba,bap,ba_edges,PURPLE)

    if ratio>=1:
        d.text((52,424),f"Edges      {er_m[0]}",font=font(14,False,True),fill=BLUE)
        d.text((52,448),f"Diameter   {er_m[1]}",font=font(14,False,True),fill=BLUE)
        d.text((220,424),f"Clustering {er_m[2]:.3f}",font=font(14,False,True),fill=BLUE)
        d.text((492,424),f"Edges      {ba_m[0]}",font=font(14,False,True),fill=PURPLE)
        d.text((492,448),f"Diameter   {ba_m[1]}",font=font(14,False,True),fill=PURPLE)
        d.text((660,424),f"Clustering {ba_m[2]:.3f}",font=font(14,False,True),fill=PURPLE)
    else:
        d.text((52,430),f"Generating G(n,p)…  {int(ratio*100):3d}%",font=font(14,False,True),fill=ACCENT)
        d.text((492,430),f"Preferential attach… {int(ratio*100):3d}%",font=font(14,False,True),fill=ACCENT)

    if i>=46:
        d.rounded_rectangle((286,82,614,112),radius=10,fill=(31,38,47))
        d.text((316,87),"diameter • clustering • degree distribution",font=font(12),fill=GREEN)
    frames.append(im)

frames[0].save("demo.gif",save_all=True,append_images=frames[1:],duration=110,loop=0,optimize=True,disposal=2)
