class_name Game
extends Node2D

@export var tilemap: AStarTileMap
@export var visualizer: TileMapLayer
@export var banana: Banana
@export var player: Player
const BANANA_SCENE = preload("res://scenes/banana.tscn")

signal object_placed

## Coloca uma banana no mapa, ou reposiciona caso já exista
func set_banana(pos: Vector2i):
	if not banana:
		banana = BANANA_SCENE.instantiate()
		add_child(banana)
	banana.position = pos
	object_placed.emit()

## Reposiciona o player
func set_player_pos(pos: Vector2i):
	player.position = pos
	object_placed.emit()

func iniciar_pathfind():
	if not banana:
		print("Não há potassio o suficiente no mapa!")
		player.pathfind_done.emit()
		return
	
	var player_pos = player.position
	player_pos = tilemap.local_to_map(player_pos)
	
	var banana_pos = banana.position
	banana_pos = tilemap.local_to_map(banana_pos)
	
	var resultado = await tilemap.A_Star(player_pos, banana_pos, tilemap.h)
	if resultado:
		Engine.time_scale = .5
		print("Caminho encontrado: ", resultado)
		player.seguir_caminho(resultado)
		Engine.time_scale = 1
	else:
		if visualizer:
			visualizer.clear()
		print("Não foi possível encontrar caminho")
		player.pathfind_done.emit()

func _ready() -> void:
	set_banana(Vector2i(64,64))
