class EscapeWatcher
{
	def EscapeWatcher(node)
	{
	}

	def update()
	{
		if (device.isKeyPressed(KeyCode_Escape, true))
		{
			device.requestShutdown();
		}
	}
}