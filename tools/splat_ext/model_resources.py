"""Non-course segment-3 banks use the same F3DEX resource layout."""
from tools.splat_ext.course_model_resources import N64SegCourse_model_resources


class N64SegModel_resources(N64SegCourse_model_resources):
    @property
    def statistics_type(self):
        return "model_resources"
