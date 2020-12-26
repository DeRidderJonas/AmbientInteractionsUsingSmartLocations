#pragma once

class NpcAgent;

class Role
{
public:
	enum class Name
	{
		Citizen = 1,
		Tourist = 2,
		Jogger = 4,
		Animal = 8,
		AnyHuman = Citizen | Tourist | Jogger,
		Any = AnyHuman | Animal
	};
	enum class Flag
	{
		DynamicJoin = 1,	//Can only join when Script has already started
		LeaveAfterEnd = 2	//Should agents move away after script has ended
	};

	Role(Name names, Flag flags, size_t cardinality_min, size_t cardinality_max);
	Role(Name names, Flag flags, size_t cardinality);
	~Role() = default;

	Name GetNames() const;
	bool IsDynamicJoin() const;

	void IncrementCardinality();
	void ResetCardinality();
	bool IsCardinalitySatisfied() const;
	bool CanAcquireAgent() const;

	void AcquireAgent(NpcAgent* pAgent);
	NpcAgent* operator[](std::size_t idx) { return m_pAgents[idx]; }
	std::vector<NpcAgent*>& GetAgents();
	void OnScriptEnd();

	inline friend Name operator| (Name a, Name b) { return (Name)((int)a | (int)b); }
	inline friend Name operator& (Name a, Name b) { return (Name)((int)a & (int)b); }
	inline friend Name operator^ (Name a, Name b) { return (Name)((int)a ^ (int)b); }
	inline friend Flag operator| (Flag a, Flag b) { return (Flag)((int)a | (int)b); }
	inline friend Flag operator& (Flag a, Flag b) { return (Flag)((int)a & (int)b); }
	inline friend Flag operator^ (Flag a, Flag b) { return (Flag)((int)a ^ (int)b); }
private:
	Name m_Names; //Bits hold wether role is active
	size_t m_Cardinality_min; //Minimum number of Actors of this role
	size_t m_Cardinality_max; //Maximum number of Actors of this role
	Flag m_Flags; //Bits hold wether role is active

	//Used while checking wether there's enough agents
	size_t m_AmountNPCs = 0;

	std::vector<NpcAgent*> m_pAgents{};
};

