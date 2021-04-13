from django.db import models

# Create your models here.
class Column_Slot(models.Model):
    column_name = models.CharField(max_length=2,default=1)

    def __str__(self) -> str:
        return self.column_name 

class Slot(models.Model):
    column_id = models.ForeignKey(Column_Slot,on_delete=models.CASCADE,default=1)
    row_name = models.CharField(max_length=2,default=1)
    is_filled = models.BooleanField(default=False)
    

    def __str__(self) -> str:
        return str(self.column_id.column_name + self.row_name)

    