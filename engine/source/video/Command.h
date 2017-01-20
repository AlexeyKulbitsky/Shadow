#ifndef SHADOW_COMMAND_INCLUDE
#define SHADOW_COMMAND_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class CommandListBase;
		class CommandContext;

		// Base struct for holding command
		struct CommandBase
		{
			CommandBase* next;
			void(*executeAndDestructPtr)(CommandListBase& commandList, CommandBase* command);

			CommandBase(void(*executeFunctionPointer)(CommandListBase& commandList, CommandBase* command))
				: next(nullptr)
				, executeAndDestructPtr(executeFunctionPointer)
			{
			}

			inline void CallExecuteAndDestroy(CommandListBase& commandList)
			{
				executeAndDestructPtr(commandList, this);
			}
		};

		// Base class for holding all command to execute
		class CommandListBase
		{
		public:
			CommandListBase(){}
			virtual ~CommandListBase(){}

			void SetCommandContext(CommandContext* context) { m_commandContext = context; }
			inline CommandContext* GetCommandContext() { return m_commandContext; }

		protected:
			CommandContext* m_commandContext;
			u8* m_commandData = nullptr;
			u32 commandDataSize = 0;
			u32 currentUsedSize = 0;
		};

		// Commands
		template<typename CommandType>
		struct Command : public CommandBase
		{
			Command()
			: CommandBase(&ExecuteAndDestruct)
			{
			}

			static void ExecuteAndDestruct(CommandListBase& commandList, CommandBase* command)
			{
				CommandType* executableCommand = static_cast<CommandType*>(command);
				executableCommand->Execute(CmdList);
				executableCommand->~();
			}
		};

		struct CommadDrawIndexedPrimitives : public Command<CommadDrawIndexedPrimitives>
		{
			void Execute(CommandListBase& commandList){}
		};
	}
}

#endif