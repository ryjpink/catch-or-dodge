"""adds record model

Revision ID: b8b2c9f25f59
Revises: 
Create Date: 2023-02-19 16:12:47.383741

"""
from alembic import op
import sqlalchemy as sa


# revision identifiers, used by Alembic.
revision = 'b8b2c9f25f59'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.create_table('record',
    sa.Column('name', sa.String(), nullable=False),
    sa.Column('score', sa.Integer(), nullable=True),
    sa.PrimaryKeyConstraint('name')
    )
    # ### end Alembic commands ###


def downgrade():
    # ### commands auto generated by Alembic - please adjust! ###
    op.drop_table('record')
    # ### end Alembic commands ###
