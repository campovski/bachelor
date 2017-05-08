class Graph:
	class Vertex:
		def __init__(self, h, e):
			self.h = h
			self.e = e
		
	class Edge:
		def __init__(self, u, v, c, f):
			self.u = u
			self.v = v
			self.c = c
			self.f = f
				
	def __init__(self, V):
		self.vertices = [self.Vertex(0, 0) for _ in range(V)]
		self.edges = []
		
	def max_flow(self):
		self.init_preflow()
		
		while self.get_overflowing_vertex() != -1:
			u = self.get_overflowing_vertex()
			if not self.push(u):
				self.relabel(u)
				
		return self.vertices[len(self.vertices)-1].e
		
	def init_preflow(self):
		self.vertices[0].h = len(self.vertices)
		
		for i in range(len(self.edges)):
			if self.edges[i].u == 0:
				self.edges[i].f = self.edges[i].c
				self.vertices[self.edges[i].v].e = self.edges[i].f
				#self.add_edge(self.edges[i].v, 0, 0, -self.edges[i].f)
				self.update_reverse_edge(i, self.edges[i].f)
	
	def push(self, u):
		for i in range(len(self.edges)):
			if self.edges[i].u == u and self.edges[i].f < self.edges[i].c\
					and self.vertices[u].h > self.vertices[self.edges[i].v].h:
				delta = min(self.vertices[u].e, self.edges[i].c - self.edges[i].f)
				self.vertices[u].e -= delta
				self.vertices[self.edges[i].v].e += delta
				self.edges[i].f += delta
				
				self.update_reverse_edge(i, delta)
				return 1
		return 0
		
	def relabel(self, u):
		min_height_adj = sys.maxint
		
		for i in range(len(self.edges)):
			if self.edges[i].u == u and self.edges[i].f < self.edges[i].c and \
					self.vertices[self.edges[i].v].h < min_height_adj:
				min_height_adj = self.vertices[self.edges[i].v].h
		
		self.vertices[u].h = min_height_adj + 1;
		
	def get_overflowing_vertex(self):
		for u in range(1, len(self.vertices)-1):
			if self.vertices[u].e > 0:
				return u
		return -1
		
	def update_reverse_edge(self, i, delta):
		for j in range(len(self.edges)):
			if self.edges[i].u == self.edges[j].v and self.edges[i].v == self.edges[j].u:
				self.edges[j].f -= delta
				return
		self.add_edge(self.edges[i].v, self.edges[i].u, 0, -delta)
		
	def add_edge(self, u, v, c, f):
		self.edges.append(self.Edge(u, v, c, f))
		

def populate_graph():
	V = int(raw_input())
	graph = Graph(V)
	
	while True:
		try:
			u, v, c = raw_input().split()
			graph.add_edge(int(u), int(v), int(c), 0)
		except EOFError:
			break
	
	return graph
			
			
if __name__ == '__main__':
	import sys
	
	graph = populate_graph()
	print "Maximum flow through given network is {0}.".format(graph.max_flow())

