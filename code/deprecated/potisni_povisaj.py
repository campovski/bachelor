class Graf:
    class Vozlisce:
        def __init__(self, h, e):
            self.h = h
            self.e = e
        
    class Povezava:
        def __init__(self, u, v, c, f):
            self.u = u
            self.v = v
            self.c = c
            self.f = f
                
    def __init__(self, V):
        self.vozlisca = [self.Vozlisce(0, 0) for _ in range(V)]
        self.povezave = []
        
    def maksimalni_pretok(self):
        self.inicializiraj_predtok()
        
        while self.vozlisce_s_presezkom() != -1:
            u = self.vozlisce_s_presezkom()
            if not self.potisni(u):
                self.povisaj(u)
                
        return self.vozlisca[len(self.vozlisca)-1].e
        
    def inicializiraj_predtok(self):
        self.vozlisca[0].h = len(self.vozlisca)
        
        for i in range(len(self.povezave)):
            if self.povezave[i].u == 0:
                self.povezave[i].f = self.povezave[i].c
                self.vozlisca[self.povezave[i].v].e =\
                    self.povezave[i].f
                #self.dodaj_povezavo(self.povezave[i].v, 0, 0, -self.povezave[i].f)
                self.posodobi_obratno_povezavo(i, self.povezave[i].f)
    
    def potisni(self, u):
        for i in range(len(self.povezave)):
            if self.povezave[i].u == u and\
                    self.povezave[i].f < self.povezave[i].c and\
                    self.vozlisca[u].h >\
                    self.vozlisca[self.povezave[i].v].h:
                delta = min(self.vozlisca[u].e,\
                    self.povezave[i].c - self.povezave[i].f)
                self.vozlisca[u].e -= delta
                self.vozlisca[self.povezave[i].v].e += delta
                self.povezave[i].f += delta
                
                self.posodobi_obratno_povezavo(i, delta)
                return 1
        return 0
        
    def povisaj(self, u):
        min_v = sys.maxint
        
        for i in range(len(self.povezave)):
            if self.povezave[i].u == u and\
                    self.povezave[i].f < self.povezave[i].c and\
                    self.vozlisca[self.povezave[i].v].h < min_v:
                min_v = self.vozlisca[self.povezave[i].v].h
        
        self.vozlisca[u].h = min_v + 1;
        
    def vozlisce_s_presezkom(self):
        for u in range(1, len(self.vozlisca)-1):
            if self.vozlisca[u].e > 0:
                return u
        return -1
        
    def posodobi_obratno_povezavo(self, i, delta):
        for j in range(len(self.povezave)):
            if self.povezave[i].u == self.povezave[j].v and\
                    self.povezave[i].v == self.povezave[j].u:
                self.povezave[j].f -= delta
                return
        self.dodaj_povezavo(\
            self.povezave[i].v, self.povezave[i].u, 0, -delta)
        
    def dodaj_povezavo(self, u, v, c, f):
        self.povezave.append(self.Povezava(u, v, c, f))
        

def napolni_graf():
    V = int(raw_input())
    G = Graf(V)
    
    while True:
        try:
            u, v, c = raw_input().split()
            G.dodaj_povezavo(int(u), int(v), int(c), 0)
        except EOFError:
            break
    
    return G
            
            
if __name__ == '__main__':
    import sys
    
    G = napolni_graf()
    print "Maksimalni pretok je {0}.".format(G.maksimalni_pretok())

