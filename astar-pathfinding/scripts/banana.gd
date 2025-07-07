class_name Banana
extends AnimatedSprite2D

func _on_area_2d_body_entered(body: Node2D) -> void:
	if body is Player:
		body.win()
		queue_free()
