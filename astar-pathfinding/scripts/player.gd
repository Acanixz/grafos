class_name Player
extends CharacterBody2D

@export var tilemap: AStarTileMap
@export var visualizer: TileMapLayer
var caminho: Array = [] 	# lista de tiles Vector2i
var index_atual = 0 		# Indice p/ onde player está indo
var andando = false
var velocidade = 100.0

signal pathfind_done

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
		fail()
		pathfind_done.emit()

func _physics_process(_delta):
	if andando and index_atual < caminho.size():
		var destino_tile: Vector2i = caminho[index_atual]
		var destino = tile_para_posicao(destino_tile)
		var direcao = (destino - global_position).normalized()
		var distancia = global_position.distance_to(destino)
		
		_update_facing(direcao)
		
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
					fail()
					recalcular_caminho()
	else:
		velocity = Vector2.ZERO

func _update_facing(direction: Vector2) -> void:
	if $Sprite.animation == "win": return
	if abs(direction.x) > abs(direction.y):
		$Sprite.animation = "right" if direction.x > 0 else "left"
	else:
		$Sprite.animation = "front" if direction.y > 0 else "back"

func win():
	$Sprite.set_animation("win")
	$Win.play()
	await $Win.finished
	$Sprite.set_animation("front")
	if visualizer:
		visualizer.clear()

func fail():
	$Sprite.modulate = Color.RED
	$SndHurt1.play()
	await $SndHurt1.finished
	$Sprite.modulate = Color.WHITE
