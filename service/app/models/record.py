from app import db

class Record(db.Model):
    name = db.Column(db.String, primary_key=True)
    score = db.Column(db.Integer)