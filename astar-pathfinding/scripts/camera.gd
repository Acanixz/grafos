extends Camera2D

@export var tilemap: TileMapLayer
var dragging := false
var last_position := Vector2.ZERO

func _ready():
	make_current()

func _input(event):
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_RIGHT:
		dragging = event.pressed
		last_position = event.position
		get_viewport().set_input_as_handled()
	elif event is InputEventMouseMotion and dragging:
		position -= event.relative
		get_viewport().set_input_as_handled()
	
