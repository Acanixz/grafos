extends Camera2D

@export var tilemap: TileMapLayer
var dragging := false
var last_position := Vector2.ZERO

func _ready():
	make_current()
	if tilemap:
		set_camera_limits_from_tilemap(tilemap)

func _input(event):
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		dragging = event.pressed
		last_position = event.position
	elif event is InputEventMouseMotion and dragging:
		position -= event.relative

func set_camera_limits_from_tilemap(tilemap: TileMapLayer):
	var used_rect = tilemap.get_used_rect()
	var top_left = tilemap.map_to_local(used_rect.position)
	var bottom_right = tilemap.map_to_local(used_rect.position + used_rect.size)

	limit_left = int(top_left.x)
	limit_top = int(top_left.y)
	limit_right = int(bottom_right.x)
	limit_bottom = int(bottom_right.y)
