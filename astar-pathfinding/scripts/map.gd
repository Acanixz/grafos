class_name AStarTileMap
extends TileMapLayer

## Permite que A* calcule caminhos diagonais
@export var diagonal_enabled = false
@export var visualizer : TileMapLayer
enum DebugColors {
	BLACK,
	WHITE,
	BLUE,
	RED,
	GREEN,
}
const MENOR_PESO = 0.8

## A* encontra um caminho de ínicio até fim.[br]
## h é a função heuristica. Onde h(n) estima o custo 
## para alcançar fim a partir do nó n
func A_Star(inicio: Vector2i, fim: Vector2i, funcH: Callable):
	# Conjunto de nós que podem precisar ser (re-)expandidos
	# Inicialmente, apenas o nó inicial é conhecido
	var open_set = [inicio]
	
	# Conjunto de nós que já foram visitados
	# Logo, não devem ser iterados novamente
	var closed_set = {}
	
	var origem = {}
	
	# g_score mantém o custo real acumulado para chegar até cada nó.
	# O nó inicial começa com custo 0, pois já começamos nele.
	# Os demais nós (incluindo o fim) assumem custo infinito até serem visitados.
	var g_score = {}
	g_score[inicio] = 0
	
	# Estimativa total do custo ao passar por cada nó
	# calculo realizado a partir de h (função heuristica)
	var f_score = {}
	f_score[inicio] = funcH.call(inicio, fim)
	
	while not open_set.is_empty():
		await get_tree().create_timer(0.1).timeout
		# Nó atual = Nó do openSet com o menor fScore
		var node_atual = open_set[0]
		var novo_node = open_set[0]
		for node in open_set:
			if f_score.get(node, INF) < f_score.get(node_atual, INF):
				novo_node = node
		
		node_atual = novo_node
		closed_set[node_atual] = true
		
		# Se chegou ao fim, reconstroi e retorna o caminho gerado
		if node_atual == fim:
			return reconstruct_path(origem, node_atual)
		
		paint_visualizer(DebugColors.BLACK, node_atual)
		open_set.erase(node_atual)
		for no_vizinho in get_neighbor_cells(node_atual):
			# Tile já visitado
			if closed_set.has(no_vizinho):
				continue
			
			# Tiles vazios ou intransitaveis são ignorados
			var tile_data_vizinho = get_cell_tile_data(no_vizinho)
			if not tile_data_vizinho or tile_data_vizinho.get_custom_data("weight") == -1:
				paint_visualizer(DebugColors.RED, no_vizinho)
				continue
			
			paint_visualizer(DebugColors.WHITE, no_vizinho)
			var tentative_g_score = g_score.get(node_atual, INF) + d(node_atual, no_vizinho)
			if tentative_g_score < g_score.get(no_vizinho, INF):
				# Esse caminho é melhor que registros anteriores, guarda ele!
				origem[no_vizinho] = node_atual
				g_score[no_vizinho] = tentative_g_score
				f_score[no_vizinho] = tentative_g_score + funcH.call(no_vizinho, fim)
				if no_vizinho not in open_set:
					open_set.append(no_vizinho)
		# paint_visualizer(DebugColors.RED, node_atual)
	# Open set ficou vazio e o fim é inalcançavel
	return null

## Retorna os nós vizinhos
func get_neighbor_cells(node_atual: Vector2i):
	var result = []
	result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_TOP_SIDE))
	result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_BOTTOM_SIDE))
	result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_LEFT_SIDE))
	result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_RIGHT_SIDE))
	
	if diagonal_enabled:
		result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_TOP_LEFT_CORNER))
		result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_TOP_RIGHT_CORNER))
		result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_BOTTOM_LEFT_CORNER))
		result.append(get_neighbor_cell(node_atual, TileSet.CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER))
	return result

## Calcula o custo real p/ ir de current até neighbor
## Leva em conta o peso do terreno (ex: areia, água, arvore)
func d(current: Vector2i, neighbor: Vector2i) -> float:
	var tile_data = get_cell_tile_data(neighbor)
	
	# Se tile não existir, retorna infinito (intransitável)
	if tile_data == null:
		return INF
	
	# Se peso for null ou -1, é intransitável
	var peso = tile_data.get_custom_data("weight")
	if peso == null or peso == -1:
		return INF
	
	# Multiplica pelo fator da distância (1 para adjacente ortogonal, ~1.41 para diagonal)
	var distancia = current.distance_to(neighbor)
	return peso * distancia

## Estima o custo mínimo p/ chegar de pos até destino
## Usa a distância direta entre os pontos multiplicado pelo menor peso conhecido
## Essa função é um chute otimista para guiar o algoritmo A* sem superestimar o custo real
func h(pos: Vector2i, destino: Vector2i) -> float:
	return pos.distance_to(destino) * MENOR_PESO

## Reconstroi o caminho A*, retorna um array que representa o caminho final
func reconstruct_path(origem: Dictionary, destino: Vector2i) -> Array:
	var caminho = [destino]
	var atual = destino

	while origem.has(atual):
		paint_visualizer(DebugColors.GREEN, atual)
		atual = origem[atual]
		caminho.append(atual)

	caminho.reverse() # porque reconstruímos de trás pra frente
	return caminho

func paint_visualizer(id: int, pos: Vector2i):
	if not visualizer or id < 0 or id > 4: return
	visualizer.set_cell(pos, 1, Vector2i(id, 0))
