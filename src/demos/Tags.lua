local tags = {
    monitor = 1 << 1,
    postProcessorStep1 = 1 << 2,
    postProcessorStep2 = 1 << 3,
    postProcessorStep3 = 1 << 4,
    postProcessorStep4 = 1 << 5,
}

tags.allPostProcessorSteps = tags.postProcessorStep1 | tags.postProcessorStep2 | tags.postProcessorStep3 | tags.postProcessorStep4

return tags