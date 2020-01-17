#include "Relation.h"

V2::Relation::Relation(const std::string& _target):
	target(_target){}

V2::Relation::Relation(const std::string& _target, EU4::EU4RelationDetails& oldRelations):
	target(_target),
	value(oldRelations.getRelations()),
	militaryAccess(oldRelations.hasMilitaryAccess()),
	lastSendDiplomat(oldRelations.getDiplomatLastSent()),
	lastWar(oldRelations.getLastWar()){}

void V2::Relation::setLevel(int lvl)
{
	if (lvl < 0 || lvl > 5) return;
	level = lvl;
}
