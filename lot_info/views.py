from django.shortcuts import render
from .models import Slot,Column_Slot
# Create your views here.

def home(request):
    context = {
        'rows':Slot.objects.all(),
        'columns':Column_Slot.objects.all()
    }
    return render(request,'lot_info/home.html',context=context)