extends CharacterBody2D

@export var tilemap: AStarTileMap
var caminho: Array = [] 	# lista de tiles Vector2i
var index_atual = 0 		# Indice p/ onde player está indo
var andando = false
var velocidade = 100.0

## Coversão de coordenadas tilemap para coordenadas reais
## (1 unidade tilemap : 32 unidades reais)
func tile_para_posicao(tile: Vector2i) -> Vector2:
	var resultado = tilemap.map_to_local(tile)
	resultado.x -= 16
	resultado.y -= 16
	return resultado

func seguir_caminho(caminho_array: Array):
	caminho = caminho_array
	index_atual = 0
	andando = true

func recalcular_caminho():
	andando = false
	var inicio = tilemap.local_to_map(global_position)
	var fim = caminho[-1] if caminho.size() > 0 else inicio
	var novo_caminho = await tilemap.A_Star(inicio, fim, tilemap.h)
	if novo_caminho:
		seguir_caminho(novo_caminho)
	else:
		print("Novo caminho não encontrado.")

func _physics_process(delta):
	if andando and index_atual < caminho.size():
		var destino_tile: Vector2i = caminho[index_atual]
		var destino = tile_para_posicao(destino_tile)
		var direcao = (destino - global_position).normalized()
		var distancia = global_position.distance_to(destino)

		if distancia > 2:  # tolerância para "chegou"
			velocity = direcao * velocidade
			move_and_slide()
		else:
			global_position = destino
			index_atual += 1

			# Verifica se próximo tile ainda é válido
			if index_atual < caminho.size():
				var proximo_tile = caminho[index_atual]
				var tile_data = tilemap.get_cell_tile_data(proximo_tile)
				if not tile_data or tile_data.get_custom_data("weight") == -1:
					print("Caminho obstruído, recalculando...")
					recalcular_caminho()
	else:
		velocity = Vector2.ZERO

func _ready() -> void:
	var inicio = Vector2i(0, -2)
	var fim = Vector2i(0, 2)
	var caminho = await tilemap.A_Star(inicio, fim, tilemap.h)
	if caminho:
		Engine.time_scale = .5
		print("Caminho encontrado: ", caminho)
		seguir_caminho(caminho)
		Engine.time_scale = 1
	else:
		print("Não foi possível encontrar caminho")
