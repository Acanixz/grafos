extends Control

@export var tilemap: AStarTileMap
@export var player: Player
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
	print("Selected tile:", selected_tile_data)

var dragging = false
func _input(event: InputEvent) -> void:
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
