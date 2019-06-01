from django.conf.urls import url
from App import views
urlpatterns = [
    url(r'^$', views.index, name='index'),
]