class_name Game
extends Node2D

@export var tilemap: AStarTileMap
@export var banana: Banana
@export var player: Player
const BANANA_SCENE = preload("res://scenes/banana.tscn")

## Coloca uma banana no mapa, ou reposiciona caso já exista
func set_banana(pos: Vector2i):
	if not banana:
		banana = BANANA_SCENE.instantiate()
		add_child(banana)
	banana.position = pos

## Reposiciona o player
func set_player_pos(pos: Vector2i):
	player.position = pos

func iniciar_pathfind():
	if not banana:
		print("Não há potassio o suficiente no mapa!")
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
		print("Não foi possível encontrar caminho")

## Define o tile correspondente
func set_tile(pos: Vector2i, id: Vector2i):
	tilemap.set_cell(pos, 0, id)

func _ready() -> void:
	set_banana(Vector2i(64,64))
	iniciar_pathfind()
