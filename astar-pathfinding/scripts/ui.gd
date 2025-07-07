extends Control

@export var tilemap: AStarTileMap
@export var visualizer: TileMapLayer
@export var player: Player
@export var game: Game
@export var build_mode:= 1
@onready var tile_grid: GridContainer = $EditorUi/TilePanel/GridContainer

var tile_set: TileSet
var selected_tile_data := {
	"source": -1,
	"coords": Vector2i.ZERO,
	"alt": 0
}

func _ready() -> void:
	visible = true
	tile_set = tilemap.tile_set
	game.object_placed.connect(_on_object_placed)
	player.pathfind_done.connect(_on_player_pathfind_done)
	
	for src_idx in tile_set.get_source_count():
		var src = tile_set.get_source(src_idx)
		if src is TileSetAtlasSource:
			var atlas_src: TileSetAtlasSource = src
			var atlas_img = atlas_src.texture.get_image()
			var count = atlas_src.get_tiles_count()
			for i in range(count):
				var coords = atlas_src.get_tile_id(i)
				var alt_count = atlas_src.get_alternative_tiles_count(coords)
				for alt in range(alt_count):
					var region = atlas_src.get_tile_texture_region(coords, alt)
					var tile_img = atlas_img.get_region(region)
					var tile_tex = ImageTexture.create_from_image(tile_img)
					
					# Build UI
					var btn = TextureButton.new()
					btn.texture_normal = tile_tex
					btn.custom_minimum_size = region.size
					btn.pressed.connect(_on_tile_selected.bind(src_idx, coords, alt))
					tile_grid.add_child(btn)

func _on_tile_selected(src:int, coords:Vector2i, alt:int) -> void:
	selected_tile_data = {"source": src, "coords": coords, "alt": alt}

var dragging = false
func _input(event: InputEvent) -> void:
	# Create/Move banana
	if build_mode == 3:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
			var local = tilemap.to_local(get_global_mouse_position())
			var coords = tilemap.local_to_map(local)
			var final_coords = tilemap.map_to_local(coords)
			final_coords.x -= 16
			final_coords.y -= 16
			game.set_banana(final_coords)
		return
	
	# Move player
	if build_mode == 2:
		if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
			var local = tilemap.to_local(get_global_mouse_position())
			var coords = tilemap.local_to_map(local)
			var final_coords = tilemap.map_to_local(coords)
			final_coords.x -= 16
			final_coords.y -= 16
			game.set_player_pos(final_coords)
		return
	
	# Create tiles
	if not build_mode == 1: return
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
		dragging = true
	
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
		dragging = false
	
	if event is InputEventMouseMotion and dragging:
		var local = tilemap.to_local(get_global_mouse_position())
		var coords = tilemap.local_to_map(local)
		tilemap.set_cell(
			coords,
			selected_tile_data.source,
			selected_tile_data.coords,
			selected_tile_data.alt
		)

func _on_move_player_btn_pressed() -> void:
	$EditorUi.visible = false
	$ConfirmEditUI/Title.text = "Clique para posicionar o jogador"
	$ConfirmEditUI.visible = true
	build_mode = 2

func _on_mov_crt_banana_btn_pressed() -> void:
	$EditorUi.visible = false
	$ConfirmEditUI/Title.text = "Clique para posicionar a banana"
	$ConfirmEditUI.visible = true
	build_mode = 3

func _on_object_placed() -> void:
	build_mode = 1
	$ConfirmEditUI.visible = false
	$EditorUi.visible = true

func _on_start_btn_pressed() -> void:
	$EditorUi.visible = false
	game.iniciar_pathfind()

func _on_player_pathfind_done() -> void:
	$EditorUi.visible = true


func _on_debug_switch_pressed() -> void:
	if visualizer:
		visualizer.visible = not visualizer.visible
		$EditorUi/ActionContainer/DebugSwitch.text = "Visão Debug: " + str(visualizer.visible)


func _on_diagonal_switch_pressed() -> void:
	tilemap.diagonal_enabled = not tilemap.diagonal_enabled
	$EditorUi/ActionContainer/DiagonalSwitch.text = "Mov. Diagonal: " + str(tilemap.diagonal_enabled)


func _on_delay_switch_pressed() -> void:
	tilemap.delay_algorithm = not tilemap.delay_algorithm
	$EditorUi/ActionContainer/DelaySwitch.text = "Atrasar A*: " + str(tilemap.delay_algorithm)
