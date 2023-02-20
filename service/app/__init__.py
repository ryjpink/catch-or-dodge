from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate

db = SQLAlchemy()
migrate = Migrate()

def create_app(test_config=None):
    app = Flask(__name__)

    app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
    app.config["SQLALCHEMY_DATABASE_URI"] = 'postgresql+psycopg2://postgres:postgres@localhost:5432/catch_or_dodge'

    db.init_app(app)
    migrate.init_app(app, db)

    from app.models.record import Record

    from app.routes import records_bp

    app.register_blueprint(records_bp)

    return app