from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse
def index(request):
	return HttpResponse("Hello, world. You're at the poll, index."+request.method)
def detail(request,question_id):
	return HttpResponse("查看id的问题")
def result(request,question_id):
	response="you're look at result of question %s."
	return HttpResponse(response%question_id)
def vote(response,question_id):
	return HttpResponse("You're voting on question %s"%question_id)
