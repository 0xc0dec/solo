class Spectator
{
	var transform;

	def Spectator(node)
	{
		this.transform := node.findComponent("Transform");
	}

	def update()
	{
		var mouseMotion = device.getMouseMotion();
		var dt = device.getTimeDelta();

		if (device.isMouseButtonDown(MouseButton_Right, true))
		{
			device.setCursorCaptured(true);
		}
		if (device.isMouseButtonReleased(MouseButton_Right))
		{
			device.setCursorCaptured(false);
		}

		if (device.isMouseButtonDown(MouseButton_Right, false))
		{
			if (mouseMotion.x != 0)
			{
				this.transform.rotate(unitVector3Y(), 0.5f * dt * -mouseMotion.x, TransformSpace_World);
			}
			if (mouseMotion.y != 0)
			{
				this.transform.rotate(unitVector3X(), 0.5f * dt * -mouseMotion.y, TransformSpace_Self);
			}
		}

		var movement = Vector3();
		if (device.isKeyPressed(KeyCode_W, false))
		{
			movement += this.transform.getLocalForward();
		}
		if (device.isKeyPressed(KeyCode_S, false))
		{
			movement += this.transform.getLocalBack();
		}
		if (device.isKeyPressed(KeyCode_A, false))
		{
			movement += this.transform.getLocalLeft();
		}
		if (device.isKeyPressed(KeyCode_D, false))
		{
			movement += this.transform.getLocalRight();
		}
		if (device.isKeyPressed(KeyCode_Q, false))
		{
			movement += this.transform.getLocalDown();
		}
		if (device.isKeyPressed(KeyCode_E, false))
		{
			movement += this.transform.getLocalUp();
		}

		movement.normalize();
		movement *= dt * 10;
		this.transform.translateLocal(movement);
	}
}