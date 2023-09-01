local module = {
	Schedules = {},
}

function module:createSchedule(name:string)
	local schedule = {
		MetaData = {
			Name = name,
		},
		Events = {
			--[[ TEMPLATE --]]
			--[[{
				Weight = 80,
				Name = 'Example',
				EventType = 'None',
				Function = function()

				end,
			},
			--]]
		},
	}
	function schedule:addEvent(event)
		if event ~= nil and event.Weight ~= nil and event.Weight~=0 and event.Function~=nil then
			table.insert(schedule.Events,event)
			function event:Trigger()
				print('Running Event From Request...')
				return event.Function()
			end
		end
	end
	function schedule:addEvents(data)
		for i,event in pairs(data) do
			if event ~= nil and event.Weight ~= nil and event.Weight~=0 and event.Function~=nil then
				table.insert(schedule.Events,event)
				function event:Trigger()
					print('Running Event From Request...')
					return event.Function()
				end
			end
		end
	end
	function schedule:removeEvent(name)
		for i,v in pairs(schedule.Events) do
			if v.Name==name then
				table.remove(schedule.Events,i)
			end
		end
	end
	function schedule:getEventFromName(name: string)
		for i,v in pairs(schedule.Events) do
			if v.Name==name then
				return v
			end
		end
	end
	function schedule:getEventFromWeight(weight)
		for i,v in pairs(schedule.Events) do
			if v.Weight==weight then
				return v
			end
		end
	end
	function schedule:getEventsFromType(t)
		local returned = {}
		for i,v in pairs(schedule.Events) do
			if v.EventType==t then
				table.insert(returned,v)
			end
		end
		return returned
	end
	function schedule:selectEvent()
		local maxWeight = 0
		for _, x in pairs(schedule.Events) do
			maxWeight = maxWeight + x.Weight
		end
		if maxWeight > 0 then
			local roll = math.random(1, maxWeight)
			local idx = 1
			while (roll > schedule.Events[idx].Weight) do
				roll = roll - schedule.Events[idx].Weight
				idx = idx + 1
			end
			return schedule.Events[idx].Function -- returns the function itself (if you add () at the end it runs the function and returns what the function returns)
		end
	end
	table.insert(module.Schedules,schedule)
	return schedule
end

function module:getScheduleFromName(name: string)
	for i,v in pairs(module.Schedules) do
		if v.MetaData.Name==name then
			return v
		end
	end
end

function module:removeSchedule(name:string)
	for i,v in pairs(module.Schedules) do
		if v.MetaData.Name==name then
			table.remove(module.Schedules,i)
		end
	end
end

return module
