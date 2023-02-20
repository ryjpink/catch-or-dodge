from app import db
from app.models.record import Record
from flask import request, Blueprint, jsonify

records_bp = Blueprint("records", __name__, url_prefix="/records")


def get_top_k(k=10, name=None):
    records = Record.query.order_by(Record.score.desc())
    record_response = []
    rank = 1
    found = True if not name else False
    for record in records:
        if rank <= k or (name and name == record.name):
            if name and name == record.name:
                found = True
            record_response.append({
                "name":record.name,
                "score":record.score,
                "rank": rank
            })
        if rank == k and found:
            break      
        rank += 1
    return record_response


@records_bp.route("", methods=["GET"])
def get_highscores():
    record_response = get_top_k()   
    return jsonify(record_response)

@records_bp.route("/<player_name>", methods=["PUT"])
def update_score(player_name):
    request_body = request.get_json()
    new_score = request_body["score"]
    record = Record.query.filter_by(name=player_name).first()
    if record:
        record.score = new_score
    else:
        record = Record(name=player_name, score=new_score)
        db.session.add(record)
    db.session.commit()
    record_response = get_top_k(name=player_name)
    return jsonify(record_response)

    

    
